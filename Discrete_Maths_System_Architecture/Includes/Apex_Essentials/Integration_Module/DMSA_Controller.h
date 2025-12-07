#pragma once
#include "File_IO_Manager.h"

// simple enum to keep track of what kind of object we are currently parsing from the file
// enum class is safer than old enums because the names dont leak out
enum class ParseState
{
	NONE, SET, RELATION, FUNCTION, GRAPH
};

// we need to look inside our function objects to see what types they use
// this structure helps us extract the domain and codomain types at compile time
template<typename T> struct Func_Traits;

// this specialization works for our function class
// it lets us say "give me the domain type of this function" in generic code
template<typename A, typename B>
struct Func_Traits<Function<A, B>> {
	using Dom = A;
	using CoDom = B;
};

// similar helper but for sets to find out what type of elements they hold
template<typename T> struct Set_Traits;
template<typename T> struct Set_Traits<Math_Set<T>> { using Type = T; };

// [NEW] Structure to capture the specific execution parameters defined in the curly braces
struct Analysis_Config
{
	std::string m_High_Value_Set;
	std::string m_Gateway_Set;      // Corresponds to 'priority_nodes' slot in user prompt
	std::string m_Freq_Set;
	std::string m_Target_Graph;
	std::string m_Start_Node;
	std::string m_End_Node;

	bool Has_Config() const { return !m_Target_Graph.empty(); }
};

class DMSA_Controller
{
private:
	std::string m_Input_File;
	std::string m_Output_File;
	Data_Registry m_Registry;
	// unique_ptr automatically deletes the memory when the controller is destroyed
	// this prevents memory leaks without needing a manual delete in the destructor
	std::unique_ptr<Output_Formatter> m_Output;

	ParseState m_Current_State = ParseState::NONE;
	std::string m_Current_Obj_Name;
	// unordered_map gives us fast o(1) access to attributes by name
	std::unordered_map<std::string, std::string> m_Current_Attributes;

	std::string m_Current_Sub_Section;

public:
	DMSA_Controller(const std::string& input, const std::string& output)
		: m_Input_File(input), m_Output_File(output)
	{
	}

	void Run()
	{
		// creates the output formatter on the heap
		m_Output = std::make_unique<Output_Formatter>(m_Output_File);

		// first pass scan to see if we need to print definitions at the top
		// wrapping in try catch because file errors might happen here
		try {
			Input_Parser<int> parser(m_Input_File);
			parser.Parse_Configuration();

			if (parser.Is_Display_Defined())
			{
				auto definitions = parser.Get_Definitions();
				m_Output->Write_Definitions(definitions);
			}
		}
		catch (...) {}

		std::ifstream file(m_Input_File);

		if (!file.is_open())
		{
			m_Output->Write_Error("Could not open input file: " + m_Input_File);
			return;
		}

		std::string line;
		int line_num = 0;

		// read the file line by line
		while (std::getline(file, line))
		{
			line_num++;
			// clean up leading and trailing spaces immediately
			std::string raw_line = String_Utils::Trim(line);
			if (raw_line.empty()) continue;

			// check if this line is a section header like --- SETS ---
			if (raw_line.find("---") == 0 || raw_line.find("===") == 0)
			{
				std::string title = String_Utils::Remove_Char(raw_line, '-');
				title = String_Utils::Remove_Char(title, '=');
				m_Output->Write_Section(title);

				m_Current_Sub_Section = "";
				continue;
			}

			// check for special directives
			std::string upper_raw = String_Utils::To_Upper(raw_line);
			if (String_Utils::Starts_With(upper_raw, "#DEFINE_DISPLAY") ||
				String_Utils::Starts_With(upper_raw, "#DISPLAY_DEFINED"))
			{
				continue;
			}

			// skip comments but allow lines that start with #define
			if (raw_line[0] == '#' && raw_line.find("#DEFINE") != 0) continue;

			try
			{
				Process_Line(raw_line);
			}
			catch (const std::exception& e)
			{
				m_Output->Write_Error("Error at line " + std::to_string(line_num) + ": " + e.what());
				m_Current_State = ParseState::NONE;
				m_Current_Attributes.clear();
			}
		}
		// ensure the last object in the file gets saved
		Flush_Current_Object();
	}

private:
	void Process_Line(const std::string& line)
	{
		std::string upper_line = String_Utils::To_Upper(line);

		// if we hit a new definition save the previous one and start fresh
		if (String_Utils::Starts_With(upper_line, "#DEFINE_"))
		{
			Flush_Current_Object();
			Start_New_Object(line);
		}
		// if we hit an operation command execute it
		else if (String_Utils::Starts_With(upper_line, "OPERATION:"))
		{
			Flush_Current_Object();
			Process_Operation(line);
		}
		// otherwise its just a property of the current object
		else
		{
			Parse_Attribute(line);
		}
	}

	void Start_New_Object(const std::string& header)
	{
		auto parts = String_Utils::Split(header, ':');
		if (parts.size() < 2) throw std::runtime_error("Invalid Header");

		std::string def_tag = String_Utils::To_Upper(String_Utils::Trim(parts[0]));
		std::string name = String_Utils::Trim(parts[1]);

		// fallback name if none provided
		if (name.empty()) name = "Obj_" + std::to_string(rand());
		m_Current_Obj_Name = name;

		// determine what kind of object this is based on the tag
		std::string type_str = def_tag.substr(8); // skip "#DEFINE_"
		if (type_str == "SET") m_Current_State = ParseState::SET;
		else if (type_str == "RELATION") m_Current_State = ParseState::RELATION;
		else if (type_str == "FUNCTION") m_Current_State = ParseState::FUNCTION;
		else if (type_str == "GRAPH") m_Current_State = ParseState::GRAPH;
		else throw std::runtime_error("Unknown Definition Type");

		m_Current_Attributes.clear();
	}

	void Parse_Attribute(const std::string& line)
	{
		// ignore attributes if we arent inside an object block
		if (m_Current_State == ParseState::NONE) return;
		auto parts = String_Utils::Split(line, ':');
		if (parts.size() < 2) return;

		std::string key = String_Utils::To_Upper(String_Utils::Trim(parts[0]));
		std::string value = String_Utils::Trim(parts[1]);
		m_Current_Attributes[key] = value;
	}

	void Flush_Current_Object()
	{
		if (m_Current_State == ParseState::NONE) return;

		ParseState state = m_Current_State;
		m_Current_State = ParseState::NONE;

		// dispatch to specific finalizer based on type
		if (state == ParseState::SET) Finalize_Set();
		else if (state == ParseState::RELATION) Finalize_Relation();
		else if (state == ParseState::FUNCTION) Finalize_Function();
		else if (state == ParseState::GRAPH) Finalize_Graph();

		m_Current_Attributes.clear();
	}

	// build a set from the gathered attributes
	void Finalize_Set()
	{
		if (!m_Current_Attributes.count("TYPE")) return;
		Data_Type type = Type_Info::From_String(m_Current_Attributes["TYPE"]);
		std::string vals = m_Current_Attributes["DATA"];

		// instantiate the correct template based on type
		if (type == Data_Type::INT) {
			auto s = Parse_Raw_Set<int>(vals);
			m_Registry.m_Sets[m_Current_Obj_Name] = s;
		}
		else if (type == Data_Type::STRING) {
			auto s = Parse_Raw_Set<std::string>(vals);
			m_Registry.m_Sets[m_Current_Obj_Name] = s;
		}
		else if (type == Data_Type::CHAR) {
			auto s = Parse_Raw_Set<char>(vals);
			m_Registry.m_Sets[m_Current_Obj_Name] = s;
		}
	}

	// build a relation
	void Finalize_Relation()
	{
		if (!m_Current_Attributes.count("DOMAIN")) return;
		std::string dom = m_Current_Attributes["DOMAIN"];
		// flexible key names for user convenience
		std::string pairs = m_Current_Attributes.count("ORDER_PAIRS") ? m_Current_Attributes["ORDER_PAIRS"] : m_Current_Attributes["PAIRS"];
		Data_Type type = m_Registry.Get_Set_Type(dom);

		if (type == Data_Type::INT) Create_Relation<int>(m_Current_Obj_Name, dom, pairs);
		else if (type == Data_Type::STRING) Create_Relation<std::string>(m_Current_Obj_Name, dom, pairs);
		else if (type == Data_Type::CHAR) Create_Relation<char>(m_Current_Obj_Name, dom, pairs);
	}

	// build a function
	// functions are trickier because domain and codomain can have different types
	void Finalize_Function()
	{
		if (!m_Current_Attributes.count("DOMAIN")) return;
		std::string dom = m_Current_Attributes["DOMAIN"];
		std::string codom = m_Current_Attributes.count("CO_DOMAIN") ? m_Current_Attributes["CO_DOMAIN"] : m_Current_Attributes["CODOMAIN"];
		std::string map = m_Current_Attributes.count("MAPPING") ? m_Current_Attributes["MAPPING"] : m_Current_Attributes["MAPPINGS"];

		Data_Type t1 = m_Registry.Get_Set_Type(dom);
		Data_Type t2 = m_Registry.Get_Set_Type(codom);

		// we have to check every possible combination of domain/codomain types
		// this is verbose but necessary because c++ templates are compile time constructs
		if (t1 == Data_Type::INT && t2 == Data_Type::INT) Create_Function<int, int>(m_Current_Obj_Name, dom, codom, map);
		else if (t1 == Data_Type::INT && t2 == Data_Type::STRING) Create_Function<int, std::string>(m_Current_Obj_Name, dom, codom, map);
		else if (t1 == Data_Type::INT && t2 == Data_Type::CHAR) Create_Function<int, char>(m_Current_Obj_Name, dom, codom, map);

		else if (t1 == Data_Type::STRING && t2 == Data_Type::INT) Create_Function<std::string, int>(m_Current_Obj_Name, dom, codom, map);
		else if (t1 == Data_Type::STRING && t2 == Data_Type::STRING) Create_Function<std::string, std::string>(m_Current_Obj_Name, dom, codom, map);
		else if (t1 == Data_Type::STRING && t2 == Data_Type::CHAR) Create_Function<std::string, char>(m_Current_Obj_Name, dom, codom, map);

		else if (t1 == Data_Type::CHAR && t2 == Data_Type::INT) Create_Function<char, int>(m_Current_Obj_Name, dom, codom, map);
		else if (t1 == Data_Type::CHAR && t2 == Data_Type::STRING) Create_Function<char, std::string>(m_Current_Obj_Name, dom, codom, map);
		else if (t1 == Data_Type::CHAR && t2 == Data_Type::CHAR) Create_Function<char, char>(m_Current_Obj_Name, dom, codom, map);
	}

	// build a graph
	void Finalize_Graph()
	{
		std::string nodes = m_Current_Attributes.count("VERTICES") ? m_Current_Attributes["VERTICES"] : m_Current_Attributes["NODES"];
		std::string edges = m_Current_Attributes["EDGES"];
		bool directed = (m_Current_Attributes.count("DIRECTED") && m_Current_Attributes["DIRECTED"] == "TRUE");
		Data_Type type = m_Registry.Get_Set_Type(nodes);

		if (type == Data_Type::INT) Create_Graph<int>(m_Current_Obj_Name, nodes, edges, directed);
		else if (type == Data_Type::STRING) Create_Graph<std::string>(m_Current_Obj_Name, nodes, edges, directed);
		else if (type == Data_Type::CHAR) Create_Graph<char>(m_Current_Obj_Name, nodes, edges, directed);
	}

	// helper functions to reduce code duplication
	// using templates allows one function to handle all data types

	template <typename T>
	void Create_Relation(const std::string& name, const std::string& dom, const std::string& pairs)
	{
		// std::get extracts the specific type from the std::variant
		Math_Set<T> domain = std::get<Math_Set<T>>(m_Registry.m_Sets[dom]);
		Relation<T> rel(domain);
		auto raw = Parse_Raw_Pairs<T>(pairs);
		for (auto& p : raw) rel.Add_Pair(p.first, p.second);
		// store back into the variant map
		m_Registry.m_Relations[name] = rel;
	}

	template <typename T1, typename T2>
	void Create_Function(const std::string& name, const std::string& dom, const std::string& codom, const std::string& mappings)
	{
		auto& setA = std::get<Math_Set<T1>>(m_Registry.m_Sets[dom]);
		auto& setB = std::get<Math_Set<T2>>(m_Registry.m_Sets[codom]);
		Function<T1, T2> func(setA, setB);

		// clean up the mapping string [1->2, 3->4]
		std::string clean = String_Utils::Remove_Char(mappings, '[');
		clean = String_Utils::Remove_Char(clean, ']');
		auto tokens = String_Utils::Split(clean, ',');
		for (const auto& t : tokens) {
			auto kv = String_Utils::Split(t, '>');
			if (kv.size() == 2) {
				std::string k = String_Utils::Remove_Char(kv[0], '-');
				func.Define_Mapping(Type_Converter::Parse_Element<T1>(k), Type_Converter::Parse_Element<T2>(kv[1]));
			}
		}
		m_Registry.m_Functions[name] = func;
	}

	template <typename T>
	void Create_Graph(const std::string& name, const std::string& nodes, const std::string& edges, bool directed)
	{
		Math_Set<T> n = std::get<Math_Set<T>>(m_Registry.m_Sets[nodes]);
		Graph<T> g(directed);
		g.Initialize_Graph(n);

		std::string clean = String_Utils::Remove_Char(edges, '[');
		clean = String_Utils::Remove_Char(clean, ']');
		// split by closing parenthesis to get each tuple (u,v,w)
		auto list = String_Utils::Split(clean, ')');

		for (auto& item : list) {
			item = String_Utils::Remove_Char(item, '(');
			auto p_raw = String_Utils::Split(item, ',');

			std::vector<std::string> p;
			for (auto& s : p_raw) {
				std::string t = String_Utils::Trim(s);
				if (!t.empty()) p.push_back(t);
			}

			if (p.size() >= 3) {
				// convert strings to correct types and add edge
				g.Add_Edge(Type_Converter::Parse_Element<T>(p[0]), Type_Converter::Parse_Element<T>(p[1]), std::stoi(p[2]));
			}
		}
		m_Registry.m_Graphs[name] = g;
	}

	// helper to print subsection headers only when the category changes
	void Update_Sub_Section(const std::string& category) {
		if (category != m_Current_Sub_Section && !category.empty()) {
			m_Current_Sub_Section = category;
			m_Output->Write_Sub_Section(category);
		}
	}

	void Process_Operation(const std::string& line)
	{
		auto parts = String_Utils::Split(line, ':');
		if (parts.size() < 2) return;

		std::string command_part = String_Utils::Trim(parts[1]);

		// Check for RUN_FULL_ANALYSIS specifically because it uses the complex {} syntax
		if (String_Utils::Starts_With(String_Utils::To_Upper(command_part), "RUN_FULL_ANALYSIS"))
		{
			Analysis_Config config;
			std::vector<std::string> generic_structures;

			// Check for config block enclosed in {}
			size_t start_brace = command_part.find('{');
			size_t end_brace = command_part.find('}');

			if (start_brace != std::string::npos && end_brace != std::string::npos && end_brace > start_brace)
			{
				// Extract config string: "high, priority, freq, graph, start, end"
				std::string config_content = command_part.substr(start_brace + 1, end_brace - start_brace - 1);
				auto config_items = String_Utils::Split(config_content, ',');

				// Map positions to struct based on prompt: 
				// {high_value_nodes, priority_nodes, frequently_accessed_nodes, graph_X, starting, ending}
				if (config_items.size() >= 6) {
					config.m_High_Value_Set = String_Utils::Trim(config_items[0]);
					config.m_Gateway_Set = String_Utils::Trim(config_items[1]); // Prompt calls this "priority_nodes", typically gateway/secondary
					config.m_Freq_Set = String_Utils::Trim(config_items[2]);
					config.m_Target_Graph = String_Utils::Trim(config_items[3]);
					config.m_Start_Node = String_Utils::Trim(config_items[4]);
					config.m_End_Node = String_Utils::Trim(config_items[5]);
				}

				// Parse the rest of the string after '}' as generic structures
				std::string remainder = command_part.substr(end_brace + 1);
				auto struct_items = String_Utils::Split(remainder, ',');
				for (const auto& s : struct_items) {
					std::string clean = String_Utils::Trim(s);
					if (!clean.empty()) generic_structures.push_back(clean);
				}
			}
			else
			{
				// Backward compatibility: Treat everything as generic structures
				// Skip "RUN_FULL_ANALYSIS" token itself (index 0)
				auto all_params = String_Utils::Split(command_part, ',');
				for (size_t i = 1; i < all_params.size(); ++i) {
					generic_structures.push_back(String_Utils::Trim(all_params[i]));
				}
			}

			Run_Full_Analysis(config, generic_structures);
			return;
		}

		// --- Standard Parsing for other operations ---
		auto params = String_Utils::Split(command_part, ',');
		std::string op = String_Utils::To_Upper(String_Utils::Trim(params[0]));

		// figure out what category this operation belongs to for the report
		std::string category = "";

		if (op == "UNION" || op == "INTERSECTION" || op == "DIFFERENCE" || op == "SYMMETRIC_DIFFERENCE") {
			category = "Basic Set Operations";
		}
		else if (op == "CARTESIAN_PRODUCT" || op == "POWER_SET") {
			category = "Advanced Set Operations";
		}
		else if (op == "IS_SUBSET" || op == "IS_PROPER_SUBSET" || op == "IS_ELEMENT") {
			category = "Set Checks";
		}
		else if (op == "QUANTIFIER_CHECK") {
			category = "Single Quantifier Analysis";
		}
		else if (op == "NESTED_QUANTIFIER_CHECK") {
			category = "Nested Quantifier Logic";
		}
		else if (op == "RELATION_CHECK") {
			std::string check = (params.size() > 2) ? String_Utils::To_Upper(String_Utils::Trim(params[2])) : "";
			if (check == "PARTIAL_ORDER" || check == "EQUIVALENCE") category = "Order & Equivalence";
			else category = "General Properties";
		}
		else if (op == "CLOSURE") {
			category = "Closures";
		}
		else if (op == "CHECK_FUNCTION" || op == "FUNCTION_CHECK") {
			category = "Properties Analysis";
		}
		else if (op == "COMPOSITION" || op == "INVERSE") {
			category = "Function Operations";
		}
		else if (op == "MST" || op == "SHORTEST_PATH") {
			category = "Pathfinding & Optimization";
		}
		else if (op == "GRAPH_CHECK") {
			category = "General Graph Properties";
		}
		else if (op == "EVALUATE" || op == "GET_IMAGES" || op == "GET_PREIMAGES") {
			category = "Evaluations";
		}

		if (!category.empty()) {
			Update_Sub_Section(category);
		}

		// execute the requested operation
		if (op == "UNION" || op == "INTERSECTION" || op == "DIFFERENCE" || op == "SYMMETRIC_DIFFERENCE")
		{
			std::string A = String_Utils::Trim(params[1]);
			std::string B = String_Utils::Trim(params[2]);
			Data_Type t = m_Registry.Get_Set_Type(A);

			// lambda to reduce code duplication for set ops
			// 'auto' allows us to pass any Math_Set<T> without explicit types
			auto DoOp = [&](auto s1, auto s2) {
				std::string label = op + "[" + A + ", " + B + "]";
				if (op == "UNION") m_Output->Write_Object(label, UNION(s1, s2));
				else if (op == "INTERSECTION") m_Output->Write_Object(label, INTERSECTION(s1, s2));
				else if (op == "DIFFERENCE") m_Output->Write_Object(label, DIFFERENCE(s1, s2));
				else m_Output->Write_Object(label, SYMMETRIC_DIFFERENCE(s1, s2));
				};

			// dispatch based on the type stored in the registry
			if (t == Data_Type::INT) DoOp(std::get<Math_Set<int>>(m_Registry.m_Sets[A]), std::get<Math_Set<int>>(m_Registry.m_Sets[B]));
			else if (t == Data_Type::STRING) DoOp(std::get<Math_Set<std::string>>(m_Registry.m_Sets[A]), std::get<Math_Set<std::string>>(m_Registry.m_Sets[B]));
			else if (t == Data_Type::CHAR) DoOp(std::get<Math_Set<char>>(m_Registry.m_Sets[A]), std::get<Math_Set<char>>(m_Registry.m_Sets[B]));
		}
		else if (op == "CARTESIAN_PRODUCT")
		{
			std::string A = String_Utils::Trim(params[1]);
			std::string B = String_Utils::Trim(params[2]);
			Run_Cartesian_Product(A, B);
		}
		else if (op == "POWER_SET")
		{
			std::string A = String_Utils::Trim(params[1]);
			Data_Type t = m_Registry.Get_Set_Type(A);
			std::string label = "POWER_SET[" + A + "]";

			if (t == Data_Type::INT) {
				auto res = POWER_SET(std::get<Math_Set<int>>(m_Registry.m_Sets[A]));
				m_Output->Write_Result(label, "Size: [" + std::to_string(res.Get_Size()) + "]");
				m_Output->Write_Object("SET", res);
			}
			else if (t == Data_Type::STRING) {
				auto res = POWER_SET(std::get<Math_Set<std::string>>(m_Registry.m_Sets[A]));
				m_Output->Write_Result(label, "Size: [" + std::to_string(res.Get_Size()) + "]");
				m_Output->Write_Object("SET", res);
			}
			else if (t == Data_Type::CHAR) {
				auto res = POWER_SET(std::get<Math_Set<char>>(m_Registry.m_Sets[A]));
				m_Output->Write_Result(label, "Size: [" + std::to_string(res.Get_Size()) + "]");
				m_Output->Write_Object("SET", res);
			}
		}
		else if (op == "IS_SUBSET" || op == "IS_PROPER_SUBSET")
		{
			std::string A = String_Utils::Trim(params[1]);
			std::string B = String_Utils::Trim(params[2]);
			Data_Type t = m_Registry.Get_Set_Type(A);
			bool res = false;
			auto check = [&](auto s1, auto s2) { return (op == "IS_SUBSET") ? IS_SUBSET(s1, s2) : IS_PROPER_SUBSET(s1, s2); };

			if (t == Data_Type::INT) res = check(std::get<Math_Set<int>>(m_Registry.m_Sets[A]), std::get<Math_Set<int>>(m_Registry.m_Sets[B]));
			else if (t == Data_Type::STRING) res = check(std::get<Math_Set<std::string>>(m_Registry.m_Sets[A]), std::get<Math_Set<std::string>>(m_Registry.m_Sets[B]));
			else if (t == Data_Type::CHAR) res = check(std::get<Math_Set<char>>(m_Registry.m_Sets[A]), std::get<Math_Set<char>>(m_Registry.m_Sets[B]));

			m_Output->Write_Property(op + "[" + A + ", " + B + "]", res);
		}
		else if (op == "IS_ELEMENT")
		{
			std::string elem = String_Utils::Trim(params[1]);
			std::string set = String_Utils::Trim(params[2]);
			Data_Type t = m_Registry.Get_Set_Type(set);
			bool res = false;

			// parse element string to the correct type then check existence
			if (t == Data_Type::INT) res = std::get<Math_Set<int>>(m_Registry.m_Sets[set]).Contains_Element(Type_Converter::Parse_Element<int>(elem));
			else if (t == Data_Type::STRING) res = std::get<Math_Set<std::string>>(m_Registry.m_Sets[set]).Contains_Element(Type_Converter::Parse_Element<std::string>(elem));
			else if (t == Data_Type::CHAR) res = std::get<Math_Set<char>>(m_Registry.m_Sets[set]).Contains_Element(Type_Converter::Parse_Element<char>(elem));

			m_Output->Write_Property("IS_ELEMENT[" + elem + ", " + set + "]", res);
		}
		else if (op == "SHORTEST_PATH")
		{
			std::string name = String_Utils::Trim(params[1]);
			std::string s = String_Utils::Trim(params[2]);
			std::string e = String_Utils::Trim(params[3]);

			if (m_Registry.Exists_Graph(name)) {
				auto& g = m_Registry.m_Graphs[name];
				// std::holds_alternative checks which type the variant contains
				if (std::holds_alternative<Graph<int>>(g)) Run_Dijkstra(std::get<Graph<int>>(g), s, e, name);
				else if (std::holds_alternative<Graph<std::string>>(g)) Run_Dijkstra(std::get<Graph<std::string>>(g), s, e, name);
				else if (std::holds_alternative<Graph<char>>(g)) Run_Dijkstra(std::get<Graph<char>>(g), s, e, name);
			}
		}
		else if (op == "MST")
		{
			std::string name = String_Utils::Trim(params[1]);
			std::string s = String_Utils::Trim(params[2]);
			std::string critSet = (params.size() > 3) ? String_Utils::Trim(params[3]) : "";

			if (m_Registry.Exists_Graph(name)) {
				auto& g = m_Registry.m_Graphs[name];
				if (std::holds_alternative<Graph<int>>(g)) Run_MST(std::get<Graph<int>>(g), s, critSet, name);
				else if (std::holds_alternative<Graph<std::string>>(g)) Run_MST(std::get<Graph<std::string>>(g), s, critSet, name);
				else if (std::holds_alternative<Graph<char>>(g)) Run_MST(std::get<Graph<char>>(g), s, critSet, name);
			}
		}
		else if (op == "RELATION_CHECK")
		{
			std::string name = String_Utils::Trim(params[1]);
			std::string check = String_Utils::Trim(params[2]);
			if (m_Registry.m_Relations.count(name)) {
				auto& r = m_Registry.m_Relations[name];
				if (std::holds_alternative<Relation<int>>(r)) Check_Rel_Props(std::get<Relation<int>>(r), check, name);
				else if (std::holds_alternative<Relation<std::string>>(r)) Check_Rel_Props(std::get<Relation<std::string>>(r), check, name);
				else if (std::holds_alternative<Relation<char>>(r)) Check_Rel_Props(std::get<Relation<char>>(r), check, name);
			}
		}
		else if (op == "CLOSURE")
		{
			std::string name = String_Utils::Trim(params[1]);
			std::string type = String_Utils::Trim(params[2]);
			if (m_Registry.Exists_Relation(name)) {
				auto& r = m_Registry.m_Relations[name];
				if (std::holds_alternative<Relation<int>>(r)) Run_Closure(std::get<Relation<int>>(r), type, name);
				else if (std::holds_alternative<Relation<std::string>>(r)) Run_Closure(std::get<Relation<std::string>>(r), type, name);
				else if (std::holds_alternative<Relation<char>>(r)) Run_Closure(std::get<Relation<char>>(r), type, name);
			}
		}
		else if (op == "GRAPH_CHECK")
		{
			std::string name = String_Utils::Trim(params[1]);
			std::string check = String_Utils::Trim(params[2]);
			if (m_Registry.m_Graphs.count(name)) {
				auto& g = m_Registry.m_Graphs[name];
				if (std::holds_alternative<Graph<int>>(g)) Check_Graph_Props(std::get<Graph<int>>(g), check, name);
				else if (std::holds_alternative<Graph<std::string>>(g)) Check_Graph_Props(std::get<Graph<std::string>>(g), check, name);
				else if (std::holds_alternative<Graph<char>>(g)) Check_Graph_Props(std::get<Graph<char>>(g), check, name);
			}
		}
		else if (op == "EVALUATE")
		{
			std::string funcName = String_Utils::Trim(params[1]);
			std::string inputVal = String_Utils::Trim(params[2]);
			if (m_Registry.m_Functions.count(funcName)) {
				auto& f_var = m_Registry.m_Functions[funcName];
				std::string label = "EVALUATE[" + funcName + ", " + inputVal + "]";

				// std::visit is the modern c++ way to inspect a variant
				// it acts like a switch statement that works on types
				// it will execute the lambda function using the actual type stored in the variant
				std::visit([&](auto& f) {
					// std::decay_t removes references and const so we get the raw type
					using FuncType = std::decay_t<decltype(f)>;
					// use our helper traits to find out what type the function expects as input
					using DomType = typename Func_Traits<FuncType>::Dom;

					try {
						// parse input string to the correct domain type
						DomType val = Type_Converter::Parse_Element<DomType>(inputVal);
						auto res = f.Evaluate_Function(val);

						// optional handling: check if we got a value back
						if (res) m_Output->Write_Result(label, Type_Converter::To_String(*res));
						else m_Output->Write_Result(label, "Undefined");
					}
					catch (...) {
						m_Output->Write_Error("Invalid input format for function domain: " + funcName);
					}
					}, f_var);
			}
		}
		else if (op == "CHECK_FUNCTION" || op == "FUNCTION_CHECK")
		{
			std::string name = String_Utils::Trim(params[1]);
			std::string check = String_Utils::Trim(params[2]);
			Run_Function_Check(name, check);
		}
		else if (op == "COMPOSITION")
		{
			std::string f_name = String_Utils::Trim(params[1]);
			std::string g_name = String_Utils::Trim(params[2]);

			if (m_Registry.Exists_Function(f_name) && m_Registry.Exists_Function(g_name)) {
				auto& v1 = m_Registry.m_Functions[f_name];
				auto& v2 = m_Registry.m_Functions[g_name];

				// visit both functions at once
				// auto& f and auto& g will be the concrete function types
				std::visit([&](auto& f, auto& g) {
					using F_T = std::decay_t<decltype(f)>;
					using G_T = std::decay_t<decltype(g)>;

					// if constexpr is evaluated at compile time
					// it checks if the codomain of f matches the domain of g
					// if not, this code block is discarded, preventing compile errors
					if constexpr (std::is_same_v<typename Func_Traits<F_T>::CoDom, typename Func_Traits<G_T>::Dom>) {
						auto res = f.Define_Composition(g);
						m_Output->Write_Object("COMPOSITION[" + f_name + ", " + g_name + "]", res);
					}
					else {
						m_Output->Write_Error("Composition Type Mismatch: " + f_name + " -> " + g_name);
					}
					}, v1, v2);
			}
		}
		else if (op == "INVERSE")
		{
			std::string name = String_Utils::Trim(params[1]);
			if (m_Registry.Exists_Function(name)) {
				auto& anyF = m_Registry.m_Functions[name];
				std::visit([&](auto& f) {
					auto inv = f.Define_Inverse();
					// optional handling again
					if (inv) m_Output->Write_Object("INVERSE[" + name + "]", *inv);
					else m_Output->Write_Error("Inverse not defined (Not Bijective)");
					}, anyF);
			}
		}
		else if (op == "GET_IMAGES")
		{
			std::string name = String_Utils::Trim(params[1]);
			if (m_Registry.Exists_Function(name)) {
				auto& anyF = m_Registry.m_Functions[name];
				std::visit([&](auto& f) {
					m_Output->Write_Object("IMAGES[" + name + "]", f.Get_Images());
					}, anyF);
			}
		}
		else if (op == "GET_PREIMAGES")
		{
			std::string name = String_Utils::Trim(params[1]);
			if (m_Registry.Exists_Function(name)) {
				auto& anyF = m_Registry.m_Functions[name];
				std::visit([&](auto& f) {
					m_Output->Write_Object("PREIMAGES[" + name + "]", f.Get_PreImages());
					}, anyF);
			}
		}
		else if (op == "QUANTIFIER_CHECK")
		{
			std::string q = String_Utils::Trim(params[1]);
			std::string set = String_Utils::Trim(params[2]);
			std::string cond = String_Utils::Trim(params[3]);

			if (m_Registry.Exists_Set(set)) {
				auto& anySet = m_Registry.m_Sets[set];

				std::visit([&](auto& s) {
					// extract the element type T from the set
					using T = typename Set_Traits<std::decay_t<decltype(s)>>::Type;

					// std::function allows us to store a lambda with a specific signature
					std::function<bool(T)> predicate;

					// use if constexpr to assign the correct lambda based on type T
					if constexpr (std::is_same_v<T, int>) {
						if (cond == "IS_POSITIVE") predicate = [](int x) { return x > 0; };
						else if (cond == "IS_EVEN") predicate = [](int x) { return x % 2 == 0; };
						else if (cond == "IS_ODD") predicate = [](int x) { return x % 2 != 0; };
					}
					else if constexpr (std::is_same_v<T, char>) {
						if (cond == "IS_UPPER") predicate = [](char c) { return std::isupper(c); };
						else if (cond == "IS_LOWER") predicate = [](char c) { return std::islower(c); };
						else if (cond == "IS_VOWEL") predicate = [](char c) { std::string v = "AEIOUaeiou"; return v.find(c) != std::string::npos; };
					}
					else if constexpr (std::is_same_v<T, std::string>) {
						if (cond == "IS_EMPTY") predicate = [](const std::string& str) { return str.empty(); };
						else if (cond == "IS_LONG") predicate = [](const std::string& str) { return str.length() > 3; };
					}

					bool res = false;
					if (predicate) {
						// call the appropriate static validator method
						if (q == "UNIVERSAL") res = Single_Quantifier_Validator<T>::Universal_Quantifier(s, predicate);
						else if (q == "EXISTENTIAL") res = Single_Quantifier_Validator<T>::Existential_Quantifier(s, predicate);
						else if (q == "UNIQUE_EXISTENTIAL") res = Single_Quantifier_Validator<T>::Unique_Existential_Quantifier(s, predicate);
						else if (q == "UNIVERSAL_DEMORGAN") res = Single_Quantifier_Validator<T>::Universal_Demorgan(s, predicate);
						else if (q == "EXISTENTIAL_DEMORGAN") res = Single_Quantifier_Validator<T>::Existential_Demorgan(s, predicate);

						m_Output->Write_Property("QUANTIFIER_" + q + "[" + set + ", " + cond + "]", res);
					}
					else {
						m_Output->Write_Error("Unknown or Incompatible Predicate: " + cond + " for set type.");
					}

					}, anySet);
			}
		}
		else if (op == "NESTED_QUANTIFIER_CHECK")
		{
			std::string type = String_Utils::Trim(params[1]);
			std::string setA = String_Utils::Trim(params[2]);
			std::string setB = String_Utils::Trim(params[3]);
			std::string pred = String_Utils::Trim(params[4]);

			if (m_Registry.Exists_Set(setA) && m_Registry.Exists_Set(setB)) {
				auto& varA = m_Registry.m_Sets[setA];
				auto& varB = m_Registry.m_Sets[setB];

				std::visit([&](auto& s1, auto& s2) {
					using T1 = typename Set_Traits<std::decay_t<decltype(s1)>>::Type;
					using T2 = typename Set_Traits<std::decay_t<decltype(s2)>>::Type;

					// enforce homogeneous types for comparison logic
					if constexpr (std::is_same_v<T1, T2>) {
						std::function<bool(T1, T1)> predicate;

						if (pred == "LESS") predicate = [](T1 a, T1 b) { return a < b; };
						else if (pred == "GREATER") predicate = [](T1 a, T1 b) { return a > b; };
						else if (pred == "EQUAL") predicate = [](T1 a, T1 b) { return a == b; };
						else if (pred == "NOT_EQUAL") predicate = [](T1 a, T1 b) { return a != b; };

						if (predicate) {
							bool res = false;
							if (type == "ALL_ALL") res = Nested_Quantifier_Validator<T1, T1>::Nested_Quantifier_Universal_Universal(s1, s2, predicate);
							else if (type == "ALL_EXISTS") res = Nested_Quantifier_Validator<T1, T1>::Nested_Quantifier_Universal_Existential(s1, s2, predicate);
							else if (type == "EXISTS_ALL") res = Nested_Quantifier_Validator<T1, T1>::Nested_Quantifier_Existential_Universal(s1, s2, predicate);
							else if (type == "EXISTS_EXISTS") res = Nested_Quantifier_Validator<T1, T1>::Nested_Quantifier_Existential_Existential(s1, s2, predicate);

							m_Output->Write_Property("NESTED_" + type + "[" + setA + ", " + setB + ", " + pred + "]", res);
						}
						else {
							m_Output->Write_Error("Unknown Predicate: " + pred);
						}
					}
					else {
						m_Output->Write_Error("Nested Quantifiers currently only support homogeneous sets.");
					}

					}, varA, varB);
			}
		}
	}

	// runs a full suite of tests on the given objects
	void Run_Full_Analysis(const Analysis_Config& config, const std::vector<std::string>& generic_args)
	{
		m_Output->Write_Section("FULL SYSTEM ANALYSIS");

		// Make a copy of generic args so we can append the target graph if needed
		std::vector<std::string> final_generic_args = generic_args;

		std::vector<std::string> sets, graphs, rels, funcs;
		Data_Type analysisType = Data_Type::UNKNOWN;

		// 1. Determine Data Type based on Config if present
		if (config.Has_Config()) {
			if (!config.m_High_Value_Set.empty() && m_Registry.Exists_Set(config.m_High_Value_Set)) {
				analysisType = m_Registry.Get_Set_Type(config.m_High_Value_Set);
			}
			else if (!config.m_Target_Graph.empty() && m_Registry.Exists_Graph(config.m_Target_Graph)) {
				// We need to peek inside the graph variant
				auto& g = m_Registry.m_Graphs[config.m_Target_Graph];
				if (std::holds_alternative<Graph<int>>(g)) analysisType = Data_Type::INT;
				else if (std::holds_alternative<Graph<std::string>>(g)) analysisType = Data_Type::STRING;
				else if (std::holds_alternative<Graph<char>>(g)) analysisType = Data_Type::CHAR;
			}

			// [AUTO-INCLUDE LOGIC]
			// Automatically add the target graph to the list of graphs to analyze generally
			// This prevents the user from having to type it twice: {..., G1, ...}, G1
			if (!config.m_Target_Graph.empty() && m_Registry.Exists_Graph(config.m_Target_Graph))
			{
				bool already_present = false;
				for (const auto& existing : final_generic_args) {
					if (existing == config.m_Target_Graph) {
						already_present = true;
						break;
					}
				}
				if (!already_present) {
					final_generic_args.push_back(config.m_Target_Graph);
				}
			}
		}

		// 2. Categorize generic inputs
		for (const auto& name : final_generic_args) {
			if (m_Registry.Exists_Set(name)) {
				sets.push_back(name);
				if (analysisType == Data_Type::UNKNOWN) analysisType = m_Registry.Get_Set_Type(name);
			}
			else if (m_Registry.Exists_Graph(name)) {
				graphs.push_back(name);
				if (analysisType == Data_Type::UNKNOWN) {
					// verify graph type using holds_alternative
					if (std::holds_alternative<Graph<int>>(m_Registry.m_Graphs[name])) analysisType = Data_Type::INT;
					else if (std::holds_alternative<Graph<std::string>>(m_Registry.m_Graphs[name])) analysisType = Data_Type::STRING;
					else if (std::holds_alternative<Graph<char>>(m_Registry.m_Graphs[name])) analysisType = Data_Type::CHAR;
				}
			}
			else if (m_Registry.Exists_Relation(name)) {
				rels.push_back(name);
			}
			else if (m_Registry.Exists_Function(name)) funcs.push_back(name);
		}

		// dispatch to the correct template instance
		if (analysisType == Data_Type::INT) Run_Full_Analysis_T<int>(config, sets, graphs, rels);
		else if (analysisType == Data_Type::STRING) Run_Full_Analysis_T<std::string>(config, sets, graphs, rels);
		else if (analysisType == Data_Type::CHAR) Run_Full_Analysis_T<char>(config, sets, graphs, rels);
		else m_Output->Write_Error("Could not determine data type for analysis or no valid objects provided.");

		if (!funcs.empty()) {
			m_Output->Write_Sub_Section("Function Analysis");
			for (const auto& fName : funcs) {
				if (m_Registry.Exists_Function(fName)) {
					Run_Function_Check(fName, "INJECTIVE");
					Run_Function_Check(fName, "SURJECTIVE");
					Run_Function_Check(fName, "BIJECTIVE");
				}
			}
		}
	}

	// generic analysis function
	// this allows us to write the logic once and reuse it for ints, strings, and chars
	template <typename T>
	void Run_Full_Analysis_T(const Analysis_Config& config, const std::vector<std::string>& sets, const std::vector<std::string>& graphs, const std::vector<std::string>& rels)
	{
		Math_Set<T> priority_nodes;

		// --- 1. Config-Driven Analysis (MST, Priority Nodes) ---
		if (config.Has_Config())
		{
			m_Output->Write_Sub_Section("Configuration Analysis");

			// Priority Node Calculation
			if (!config.m_High_Value_Set.empty() && !config.m_Freq_Set.empty() && !config.m_Gateway_Set.empty())
			{
				if (m_Registry.Exists_Set(config.m_High_Value_Set) &&
					m_Registry.Exists_Set(config.m_Freq_Set) &&
					m_Registry.Exists_Set(config.m_Gateway_Set))
				{
					auto s1 = std::get<Math_Set<T>>(m_Registry.m_Sets[config.m_High_Value_Set]);
					auto s2 = std::get<Math_Set<T>>(m_Registry.m_Sets[config.m_Freq_Set]);
					auto s3 = std::get<Math_Set<T>>(m_Registry.m_Sets[config.m_Gateway_Set]);

					// Intersection of High Value and Freq
					auto intersection = INTERSECTION(s1, s2);
					// Union with Gateway
					priority_nodes = UNION(intersection, s3);

					m_Output->Write_Object("PRIORITY_NODES_CALCULATION", priority_nodes);
				}
				else {
					m_Output->Write_Error("One or more config sets not found in registry.");
				}
			}

			// Graph Analysis (MST & Shortest Path) on Target Graph
			if (!config.m_Target_Graph.empty() && m_Registry.Exists_Graph(config.m_Target_Graph))
			{
				auto& graph = std::get<Graph<T>>(m_Registry.m_Graphs[config.m_Target_Graph]);

				// Shortest Path
				if (!config.m_Start_Node.empty() && !config.m_End_Node.empty()) {
					try {
						Run_Dijkstra(graph, config.m_Start_Node, config.m_End_Node, config.m_Target_Graph);
					}
					catch (...) { m_Output->Write_Error("Shortest Path Failed"); }
				}

				// MST with Constraints
				if (!config.m_Start_Node.empty() && priority_nodes.Get_Size() > 0) {
					auto res = Constrained_Prims_MST<T>::Constrained_Prims_MST_Calculation(graph, priority_nodes, Type_Converter::Parse_Element<T>(config.m_Start_Node));
					m_Output->Write_Object("MST[" + config.m_Target_Graph + ", Constraints: PriorityNodes]", res);
				}
			}
		}
		// --- Backward Compatibility for Implicit Sets ---
		else if (sets.size() >= 3) {
			m_Output->Write_Sub_Section("Legacy Set Operations");
			auto s1 = std::get<Math_Set<T>>(m_Registry.m_Sets[sets[0]]);
			auto s2 = std::get<Math_Set<T>>(m_Registry.m_Sets[sets[1]]);
			auto s3 = std::get<Math_Set<T>>(m_Registry.m_Sets[sets[2]]);
			Math_Set<T> intersection = INTERSECTION(s1, s2);
			priority_nodes = UNION(intersection, s3);
			m_Output->Write_Object("PRIORITY_NODES[" + sets[0] + " n " + sets[1] + " U " + sets[2] + "]", priority_nodes);
		}

		// --- 2. Generic Structure Analysis (Iterate over the lists) ---

		if (!graphs.empty()) {
			m_Output->Write_Sub_Section("Graph Analysis"); // Broad Header
			for (const auto& graphName : graphs) {
				if (std::holds_alternative<Graph<T>>(m_Registry.m_Graphs[graphName])) {
					auto& graph = std::get<Graph<T>>(m_Registry.m_Graphs[graphName]);

					// Create a specific sub-header or spacer for this graph
					m_Output->Write_Raw("\nAnalysis for Graph: " + graphName);
					m_Output->Write_Raw("------------------------------------------------------------");

					m_Output->Write_Info("Vertex Count", std::to_string(graph.Get_Num_Vertices()));
					m_Output->Write_Info("Edge Count", std::to_string(graph.Get_Num_Edges()));

					// Check all requested properties
					Check_Graph_Props(graph, "IS_DIRECTED", graphName);
					Check_Graph_Props(graph, "CONNECTED_COMPONENTS", graphName);
					Check_Graph_Props(graph, "COMPLETE", graphName);
					Check_Graph_Props(graph, "EULERIAN_CIRCUIT", graphName);
					Check_Graph_Props(graph, "EULERIAN_PATH", graphName);
					Check_Graph_Props(graph, "HAMILTONIAN_CYCLE", graphName);
				}
			}
		}

		if (!rels.empty()) {
			m_Output->Write_Sub_Section("Relation Analysis");
			for (const auto& rName : rels) {
				if (std::holds_alternative<Relation<T>>(m_Registry.m_Relations[rName])) {
					auto& rel = std::get<Relation<T>>(m_Registry.m_Relations[rName]);

					m_Output->Write_Raw("\nAnalysis for Relation: " + rName);
					m_Output->Write_Raw("------------------------------------------------------------");

					Check_Rel_Props(rel, "REFLEXIVE", rName);
					Check_Rel_Props(rel, "SYMMETRIC", rName);
					Check_Rel_Props(rel, "ANTISYMMETRIC", rName);
					Check_Rel_Props(rel, "TRANSITIVE", rName);
					Check_Rel_Props(rel, "EQUIVALENCE", rName);
					Check_Rel_Props(rel, "PARTIAL_ORDER", rName);
				}
			}
		}
	}

	// helper to parse raw string data into a set
	template <typename T>
	Math_Set<T> Parse_Raw_Set(std::string content) {
		Math_Set<T> r;
		content = String_Utils::Remove_Char(content, '{'); content = String_Utils::Remove_Char(content, '}');
		auto el = String_Utils::Split(content, ',');
		for (auto e : el) {
			std::string clean_e = String_Utils::Trim(e);
			if (!clean_e.empty()) {
				r.Insert_Element(Type_Converter::Parse_Element<T>(clean_e));
			}
		}
		return r;
	}

	// helper to parse raw string data into a list of pairs
	template <typename T>
	std::vector<std::pair<T, T>> Parse_Raw_Pairs(std::string content) {
		std::vector<std::pair<T, T>> res;
		content = String_Utils::Remove_Char(content, '['); content = String_Utils::Remove_Char(content, ']');
		auto parts = String_Utils::Split(content, ')');
		for (auto p : parts) {
			p = String_Utils::Remove_Char(p, '(');
			auto nums = String_Utils::Split(p, ',');

			std::vector<std::string> clean;
			for (auto n : nums) { n = String_Utils::Trim(n); if (!n.empty()) clean.push_back(n); }
			if (clean.size() >= 2) res.push_back({ Type_Converter::Parse_Element<T>(clean[0]), Type_Converter::Parse_Element<T>(clean[1]) });
		}
		return res;
	}

	template<typename T>
	void Check_Rel_Props(const Relation<T>& rel, std::string check, std::string name) {
		std::string label = check + "[" + name + "]";
		if (check == "REFLEXIVE") m_Output->Write_Property(label, rel.Is_Reflexive());
		else if (check == "SYMMETRIC") m_Output->Write_Property(label, rel.Is_Symmetric());
		else if (check == "ANTISYMMETRIC") m_Output->Write_Property(label, rel.Is_AntiSymmetric());
		else if (check == "TRANSITIVE") m_Output->Write_Property(label, rel.Is_Transitive());
		else if (check == "EQUIVALENCE") m_Output->Write_Property(label, rel.Is_Equivalence_Relation());
		else if (check == "PARTIAL_ORDER") m_Output->Write_Property(label, rel.Is_Partial_Order());
	}

	template<typename T>
	void Run_Closure(const Relation<T>& rel, std::string type, std::string name) {
		Relation<T> res;
		if (type == "REFLEXIVE") res = rel.Reflexive_Closure();
		else if (type == "SYMMETRIC") res = rel.Symmetric_Closure();
		else if (type == "TRANSITIVE") res = rel.Transitive_Closure();
		m_Output->Write_Object(type + "_CLOSURE[" + name + "]", res);
	}

	template<typename T>
	void Check_Graph_Props(const Graph<T>& graph, std::string check, std::string name) {
		std::string label = check + "[" + name + "]";
		if (check == "COMPLETE") m_Output->Write_Property(label, graph.Is_Complete());
		else if (check == "EULERIAN_CIRCUIT") m_Output->Write_Property(label, graph.Is_Eulerian_Circuit());
		else if (check == "EULERIAN_PATH") m_Output->Write_Property(label, graph.Is_Eulerian_Path());
		else if (check == "HAMILTONIAN_CYCLE") m_Output->Write_Property(label, graph.Has_Hamiltonian_Cycle());
		else if (check == "CONNECTED_COMPONENTS") m_Output->Write_Result(label, std::to_string(graph.Count_Connected_Components()));
		else if (check == "IS_DIRECTED") m_Output->Write_Property(label, graph.Is_Directed());
	}

	template<typename T>
	void Run_Dijkstra(const Graph<T>& graph, std::string startStr, std::string endStr, std::string name) {
		T start = Type_Converter::Parse_Element<T>(startStr);
		T end = Type_Converter::Parse_Element<T>(endStr);
		auto res = Dijikstras_Shortest_Path<T>::Dijikstras_Shortest_Path_Calculation(graph, start, end);
		m_Output->Write_Object("SHORTEST_PATH[" + name + ", " + startStr + "->" + endStr + "]", res);
	}

	template<typename T>
	void Run_MST(const Graph<T>& graph, std::string startStr, std::string critSetName, std::string name) {
		T start = Type_Converter::Parse_Element<T>(startStr);
		Math_Set<T> constraints;
		if (!critSetName.empty() && m_Registry.Exists_Set(critSetName)) {
			if (std::holds_alternative<Math_Set<T>>(m_Registry.m_Sets[critSetName])) constraints = std::get<Math_Set<T>>(m_Registry.m_Sets[critSetName]);
		}
		auto res = Constrained_Prims_MST<T>::Constrained_Prims_MST_Calculation(graph, constraints, start);
		m_Output->Write_Object("MST[" + name + ", Start:" + startStr + "]", res);
	}

	void Run_Function_Check(std::string name, std::string check) {
		if (!m_Registry.Exists_Function(name)) return;
		auto& anyF = m_Registry.m_Functions[name];
		auto do_check = [&](auto& f) {
			std::string label = check + "[" + name + "]";
			if (check == "INJECTIVE") m_Output->Write_Property(label, f.Is_Injective());
			else if (check == "SURJECTIVE") m_Output->Write_Property(label, f.Is_Surjective());
			else if (check == "BIJECTIVE") m_Output->Write_Property(label, f.Is_Bijective());
			else if (check == "PROPER") m_Output->Write_Property(label, f.Is_Proper_Function());
			};
		std::visit(do_check, anyF);
	}

	// helper to run cartesian product on any combination of types
	// hardcoded permutations for all 9 combinations of int/string/char
	void Run_Cartesian_Product(std::string setA, std::string setB) {
		Data_Type tA = m_Registry.Get_Set_Type(setA);
		Data_Type tB = m_Registry.Get_Set_Type(setB);
		std::string label = "CARTESIAN_PRODUCT[" + setA + ", " + setB + "]";

		// 1. INT x INT
		if (tA == Data_Type::INT && tB == Data_Type::INT) {
			auto res = CARTESIAN_PRODUCT(std::get<Math_Set<int>>(m_Registry.m_Sets[setA]), std::get<Math_Set<int>>(m_Registry.m_Sets[setB]));
			m_Output->Write_Object(label, res);
		}
		// 2. INT x STRING
		else if (tA == Data_Type::INT && tB == Data_Type::STRING) {
			auto res = CARTESIAN_PRODUCT(std::get<Math_Set<int>>(m_Registry.m_Sets[setA]), std::get<Math_Set<std::string>>(m_Registry.m_Sets[setB]));
			m_Output->Write_Object(label, res);
		}
		// 3. INT x CHAR
		else if (tA == Data_Type::INT && tB == Data_Type::CHAR) {
			auto res = CARTESIAN_PRODUCT(std::get<Math_Set<int>>(m_Registry.m_Sets[setA]), std::get<Math_Set<char>>(m_Registry.m_Sets[setB]));
			m_Output->Write_Object(label, res);
		}
		// 4. STRING x INT
		else if (tA == Data_Type::STRING && tB == Data_Type::INT) {
			auto res = CARTESIAN_PRODUCT(std::get<Math_Set<std::string>>(m_Registry.m_Sets[setA]), std::get<Math_Set<int>>(m_Registry.m_Sets[setB]));
			m_Output->Write_Object(label, res);
		}
		// 5. STRING x STRING
		else if (tA == Data_Type::STRING && tB == Data_Type::STRING) {
			auto res = CARTESIAN_PRODUCT(std::get<Math_Set<std::string>>(m_Registry.m_Sets[setA]), std::get<Math_Set<std::string>>(m_Registry.m_Sets[setB]));
			m_Output->Write_Object(label, res);
		}
		// 6. STRING x CHAR
		else if (tA == Data_Type::STRING && tB == Data_Type::CHAR) {
			auto res = CARTESIAN_PRODUCT(std::get<Math_Set<std::string>>(m_Registry.m_Sets[setA]), std::get<Math_Set<char>>(m_Registry.m_Sets[setB]));
			m_Output->Write_Object(label, res);
		}
		// 7. CHAR x INT
		else if (tA == Data_Type::CHAR && tB == Data_Type::INT) {
			auto res = CARTESIAN_PRODUCT(std::get<Math_Set<char>>(m_Registry.m_Sets[setA]), std::get<Math_Set<int>>(m_Registry.m_Sets[setB]));
			m_Output->Write_Object(label, res);
		}
		// 8. CHAR x STRING
		else if (tA == Data_Type::CHAR && tB == Data_Type::STRING) {
			auto res = CARTESIAN_PRODUCT(std::get<Math_Set<char>>(m_Registry.m_Sets[setA]), std::get<Math_Set<std::string>>(m_Registry.m_Sets[setB]));
			m_Output->Write_Object(label, res);
		}
		// 9. CHAR x CHAR
		else if (tA == Data_Type::CHAR && tB == Data_Type::CHAR) {
			auto res = CARTESIAN_PRODUCT(std::get<Math_Set<char>>(m_Registry.m_Sets[setA]), std::get<Math_Set<char>>(m_Registry.m_Sets[setB]));
			m_Output->Write_Object(label, res);
		}
	}
};