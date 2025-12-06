#pragma once

#include "Data_Registery.h"

// Input_Parser class parses an input file and converts textual representations
// into program data types using templates so it works with any comparable type
template<Comparable type>
class Input_Parser
{
private:
	std::ifstream m_Input_File; // file stream used to read from disk
	std::string m_File_Name;    // store filename for messages and errors
	Data_Type m_Current_Type;   // the data type declared in the input file
	std::string m_Current_Module; // name of the module section in the file
	bool m_Display_Defined;     // flag to trigger printing of definitions

public:
	// constructor opens the input file and throws on failure
	Input_Parser(const std::string& filename)
		: m_File_Name(filename), m_Current_Type(Data_Type::UNKNOWN), m_Display_Defined(false)
	{
		m_Input_File.open(filename); // open file stream for subsequent parsing
		if (!m_Input_File.is_open())
		{
			// if file cannot be opened we stop here and report the reason to caller
			throw std::runtime_error("Failed to open input file: " + filename);
		}
	}

	// destructor closes file if still open
	~Input_Parser()
	{
		if (m_Input_File.is_open())
		{
			m_Input_File.close(); // release system resource now that parser is done
		}
	}

	// Parse_Configuration reads header lines to extract declared type and module
	bool Parse_Configuration()
	{
		std::string line;
		Seek_Beginning(); // reset file cursor before scanning file

		while (std::getline(m_Input_File, line))
		{
			line = String_Utils::Trim(line); // remove leading and trailing whitespace

			if (line.empty()) continue;

			// Check for special directive #DEFINE_DISPLAY (or #DISPLAY_DEFINED)
			std::string upper_line = String_Utils::To_Upper(line);
			if (upper_line == "#DEFINE_DISPLAY" || upper_line == "#DISPLAY_DEFINED")
			{
				m_Display_Defined = true;
				continue;
			}

			if (line[0] == '#') continue; // skip comments

			auto tokens = String_Utils::Split(line, ':'); // key value pairs separated by colon
			if (tokens.size() != 2) continue; // not a key value line so ignore it

			std::string key = String_Utils::To_Upper(tokens[0]); // normalize key to upper case
			std::string value = String_Utils::Trim(tokens[1]); // value part trimmed of whitespace

			if (key == "TYPE")
			{
				// convert text type to enum used by program
				m_Current_Type = Type_Info::From_String(value);
				// ensure the type resolved is valid for the program
				if (!Type_Info::Is_Valid(m_Current_Type))
				{
					// invalid type is a fatal configuration error
					throw std::runtime_error("Invalid type specified: " + value);
				}
			}
			else if (key == "MODULE")
			{
				// store module name in uppercase for consistent matching later
				m_Current_Module = String_Utils::To_Upper(value);
			}
		}

		// return whether we found a valid type in configuration
		return Type_Info::Is_Valid(m_Current_Type);
	}

	// Getter for the display flag
	bool Is_Display_Defined() const
	{
		return m_Display_Defined;
	}

	// Scans the file for all structure definitions and groups them by category
	// Returns a map: Category -> Vector of {Name, DetailsString}
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> Get_Definitions()
	{
		if (!m_Display_Defined) return {};

		std::map<std::string, std::vector<std::pair<std::string, std::string>>> grouped_defs;
		Seek_Beginning();
		std::string line;

		std::string current_category = "";
		std::string current_name = "";
		std::stringstream current_details;

		auto Flush = [&]() {
			if (!current_name.empty() && !current_category.empty()) {
				grouped_defs[current_category].push_back({ current_name, current_details.str() });
			}
			current_name = "";
			current_details.str("");
			current_details.clear();
			};

		while (std::getline(m_Input_File, line))
		{
			std::string raw_line = String_Utils::Trim(line);
			if (raw_line.empty()) continue;

			std::string upper_line = String_Utils::To_Upper(raw_line);

			if (String_Utils::Starts_With(upper_line, "#DEFINE_"))
			{
				Flush(); // Save previous object

				auto parts = String_Utils::Split(raw_line, ':');
				if (parts.size() >= 2) {
					current_name = String_Utils::Trim(parts[1]);
					std::string type = String_Utils::Trim(parts[0]);
					type = String_Utils::To_Upper(type);

					if (type.find("SET") != std::string::npos) current_category = "SETS";
					else if (type.find("RELATION") != std::string::npos) current_category = "RELATIONS";
					else if (type.find("FUNCTION") != std::string::npos) current_category = "FUNCTIONS";
					else if (type.find("GRAPH") != std::string::npos) current_category = "GRAPH";
				}
			}
			else if (String_Utils::Starts_With(upper_line, "OPERATION:") || raw_line[0] == '#' || raw_line.find("---") == 0)
			{
				Flush(); // End of definitions block or start of operations
				current_category = ""; // Stop collecting
			}
			else if (!current_category.empty() && !current_name.empty())
			{
				// Collect attributes for the current object (e.g., "Data: {1,2}")
				current_details << raw_line << " ";
			}
		}
		Flush(); // Final flush

		return grouped_defs;
	}

	// Parse_Set converts a textual set like {1, 2, 3} into Math_Set<type>
	Math_Set<type> Parse_Set(const std::string& set_string)
	{
		Math_Set<type> result; // container to collect parsed elements

		std::string content = set_string;
		content = String_Utils::Remove_Char(content, '{'); // remove opening brace
		content = String_Utils::Remove_Char(content, '}'); // remove closing brace
		content = String_Utils::Remove_Char(content, '['); // accept bracket variants
		content = String_Utils::Remove_Char(content, ']'); // remove closing bracket

		auto elements = String_Utils::Split(content, ','); // elements separated by commas

		for (const auto& elem_str_raw : elements)
		{
			std::string elem_str = String_Utils::Trim(elem_str_raw); // trim each token
			if (elem_str.empty()) continue; // skip empty entries produced by commas

			// parse element text into the generic type using a conversion utility
			type element = Type_Converter::Parse_Element<type>(elem_str);
			result.Insert_Element(element); // insert parsed element into result set
		}

		return result; // return constructed set to caller
	}

	// Parse_Pair converts textual pair like (A,B) into std::pair<type,type>
	std::pair<type, type> Parse_Pair(const std::string& pair_string)
	{
		std::string content = pair_string;
		content = String_Utils::Remove_Char(content, '('); // strip opening parenthesis
		content = String_Utils::Remove_Char(content, ')'); // strip closing parenthesis

		auto parts = String_Utils::Split(content, ','); // left and right parts separated by comma
		if (parts.size() != 2)
		{
			// enforce exact pair format, otherwise report to caller
			throw std::runtime_error("Invalid pair format: " + pair_string);
		}

		// parse each side into the requested template type
		type first = Type_Converter::Parse_Element<type>(String_Utils::Trim(parts[0]));
		type second = Type_Converter::Parse_Element<type>(String_Utils::Trim(parts[1]));

		return { first, second };
	}

	// Parse_Pair_List finds all pairs in a bracketed list [(1,2),(2,3)]
	std::vector<std::pair<type, type>> Parse_Pair_List(const std::string& list_string)
	{
		std::vector<std::pair<type, type>> pairs; // collect parsed pairs here

		std::string content = list_string;
		content = String_Utils::Remove_Char(content, '['); // remove opening list bracket
		content = String_Utils::Remove_Char(content, ']'); // remove closing list bracket

		size_t pos = 0; // search position in string
		while (pos < content.length())
		{
			size_t start = content.find('(', pos); // find a pair start
			if (start == std::string::npos) break; // no more pairs

			size_t end = content.find(')', start); // find matching pair end
			if (end == std::string::npos) break; // malformed list without closing parenthesis

			std::string pair_str = content.substr(start, end - start + 1); // include parentheses
			pairs.push_back(Parse_Pair(pair_str)); // parse and append pair

			pos = end + 1; // continue searching after this pair
		}

		return pairs; // return all parsed pairs
	}

	// Parse_Mapping converts notation like A->B into a pair representing mapping
	std::pair<type, type> Parse_Mapping(const std::string& mapping_string)
	{
		auto parts = String_Utils::Split(mapping_string, '>'); // split at greater than char
		if (parts.size() != 2)
		{
			// mapping must contain exactly one arrow symbol '->'
			throw std::runtime_error("Invalid mapping format: " + mapping_string);
		}

		std::string first_part = String_Utils::Remove_Char(parts[0], '-'); // remove dash from left side

		// parse left and right side using conversion helper
		type input = Type_Converter::Parse_Element<type>(String_Utils::Trim(first_part));
		type output = Type_Converter::Parse_Element<type>(String_Utils::Trim(parts[1]));

		return { input, output };
	}

	// Get_All_Sets scans file and returns any keys that declare sets
	std::unordered_map<std::string, Math_Set<type>> Get_All_Sets()
	{
		std::unordered_map<std::string, Math_Set<type>> sets; // result map keyed by the label from file
		Seek_Beginning(); // start scanning from file start

		std::string line;
		while (std::getline(m_Input_File, line))
		{
			line = String_Utils::Trim(line); // trim whitespace for consistent parsing
			if (line.empty() || line[0] == '#') continue; // ignore comments and blanks

			auto tokens = String_Utils::Split(line, ':'); // expect key : value format
			if (tokens.size() != 2) continue; // skip lines that do not match expected format

			std::string key = String_Utils::To_Upper(tokens[0]); // normalize key for map storage
			std::string value = String_Utils::Trim(tokens[1]); // get the textual set description

			if (String_Utils::Starts_With(key, "SET_") || key == "DOMAIN" || key == "CODOMAIN")
			{
				try
				{
					sets[key] = Parse_Set(value); // attempt to parse set and store it
				}
				catch (const std::exception& e)
				{
					// parsing failed for this key we report error but continue processing
					std::cerr << "Error parsing " << key << ": " << e.what() << std::endl;
				}
			}
		}

		return sets; // return map of all discovered sets
	}

	// Get_Commands returns lines that represent operations or checks to be executed
	std::vector<std::string> Get_Commands()
	{
		std::vector<std::string> commands; // collect candidate command lines
		Seek_Beginning(); // reset file cursor to scan commands

		std::string line;
		while (std::getline(m_Input_File, line))
		{
			line = String_Utils::Trim(line); // normalize whitespace
			if (line.empty() || line[0] == '#') continue; // skip comments and blanks

			std::string upper_line = String_Utils::To_Upper(line); // case independent checks

			// check if this line starts with any supported operation keyword
			if (String_Utils::Starts_With(upper_line, "OPERATION:") ||
				String_Utils::Starts_With(upper_line, "CHECK:") ||
				String_Utils::Starts_With(upper_line, "COMPUTE:") ||
				String_Utils::Starts_With(upper_line, "CLOSURE:") ||
				String_Utils::Starts_With(upper_line, "SHORTEST_PATH:") ||
				String_Utils::Starts_With(upper_line, "MST:"))
			{
				commands.push_back(line); // push original line preserving case for downstream parsing
			}
		}

		return commands; // return all operation lines found
	}

	// Get_Value finds the first occurrence of a key and returns its value as text
	std::string Get_Value(const std::string& key)
	{
		Seek_Beginning(); // scan file from start for simple key lookup

		std::string line;
		std::string search_key = String_Utils::To_Upper(key); // case insensitive search

		while (std::getline(m_Input_File, line))
		{
			line = String_Utils::Trim(line); // trim whitespace for reliable tokenization
			if (line.empty() || line[0] == '#') continue; // ignore comments and blanks

			auto tokens = String_Utils::Split(line, ':'); // expect key : value pairs
			if (tokens.size() != 2) continue; // skip lines that do not match

			std::string current_key = String_Utils::To_Upper(tokens[0]); // normalize token for matching

			if (current_key == search_key)
			{
				return String_Utils::Trim(tokens[1]); // return trimmed value portion
			}
		}

		return ""; // not found return empty string
	}

	// Get_Type returns the data type parsed from the file header
	Data_Type Get_Type() const
	{
		return m_Current_Type; // simple accessor needed by callers
	}

	// Get_Module returns module name extracted from header
	std::string Get_Module() const
	{
		return m_Current_Module; // return stored module label
	}

private:
	// Seek_Beginning resets error flags and moves file cursor to beginning
	void Seek_Beginning()
	{
		m_Input_File.clear(); // clear eof and error flags so future reads succeed
		m_Input_File.seekg(0, std::ios::beg); // move cursor to start of file for fresh scan
	}
};

// Output_Formatter formats results and writes them to a file and optionally to console
class Output_Formatter
{
private:
	std::ofstream m_Output_File;
	std::string m_File_Name;
	int m_Test_Count = 0;
	int m_Test_Passed = 0;
	int m_Test_Failed = 0;
	bool m_Console_Echo;

	const int LABEL_WIDTH = 60; // Standard width for alignment

public:
	Output_Formatter(const std::string& filename, bool console_echo = true)
		: m_File_Name(filename), m_Console_Echo(console_echo)
	{
		m_Output_File.open(filename);
		if (!m_Output_File.is_open())
		{
			throw std::runtime_error("Failed to open output file: " + filename);
		}
		Write_Main_Header();
	}

	~Output_Formatter()
	{
		Write_Summary();
		if (m_Output_File.is_open()) m_Output_File.close();
	}

	void Write_Main_Header()
	{
		std::string border(120, '=');
		Write_Line(border);
		Write_Line("  DISCRETE MATHEMATICS AUTOMATION SYSTEM - EXECUTION REPORT");
		Write_Line(border);
		Write_Line("");
	}

	void Write_Section(const std::string& title)
	{
		std::string clean_title = String_Utils::Trim(title);
		if (clean_title.empty()) return; // Don't write empty sections

		std::string border(120, '=');

		// Add extra vertical spacing before sections
		Write_Line("");
		Write_Line("");
		Write_Line(border);
		Write_Line("  SECTION: " + clean_title);
		Write_Line(border);
		Write_Line(""); // Spacing after section header
	}

	void Write_Sub_Section(const std::string& title)
	{
		std::string clean_title = String_Utils::Trim(title);
		if (clean_title.empty()) return; // Don't write empty sub-sections

		std::string border(120, '-');
		Write_Line("");
		Write_Line(border);
		Write_Line("  " + clean_title);
		Write_Line(border);
	}

	// Prints a dedicated section for defined structures (Sets, Relations, etc.)
	// Triggered if #DISPLAY_DEFINED is present in input
	void Write_Definitions(const std::map<std::string, std::vector<std::pair<std::string, std::string>>>& groups)
	{
		if (groups.empty()) return;

		// No Main SECTION header here, we print group headers directly

		// Order of display: SETS, GRAPH, RELATIONS, FUNCTIONS
		std::vector<std::string> order = { "SETS", "GRAPH", "RELATIONS", "FUNCTIONS" };

		for (const auto& cat : order) {
			if (groups.count(cat)) {
				Write_Sub_Section(cat); // e.g. "SETS"
				for (const auto& item : groups.at(cat)) {
					// We need to parse details string to separate key-values if possible
					// But for now, we print Name: Details
					// Format: name:details
					std::stringstream ss;
					ss << item.first << ":" << item.second;
					Write_Line(ss.str());
				}
			}
		}
	}

	// Streams ANY object using its operator<<.
	template <typename T>
	void Write_Object(const std::string& label, const T& object)
	{
		m_Test_Count++;
		m_Test_Passed++;

		std::stringstream ss;
		ss << object;
		std::string content = ss.str();

		// Formatting: JUST the Label
		std::string header = label;

		// If content is multi-line (Graphs, large sets), put it on the next line
		if (content.find('\n') != std::string::npos)
		{
			Write_Line(header + ":");
			Write_Line(content);
			Write_Line("------------------------------------------------------------");
			Write_Line(""); // Trailing space
		}
		else
		{
			// Single line objects
			std::stringstream line_ss;
			line_ss << std::left << std::setw(LABEL_WIDTH) << (header + " :") << content;
			Write_Line(line_ss.str());
		}
	}

	// Special Write for just strings (used in custom reports)
	void Write_Info(const std::string& label, const std::string& content)
	{
		std::stringstream line_ss;
		line_ss << std::left << std::setw(LABEL_WIDTH) << (label + " :") << content;
		Write_Line(line_ss.str());
	}

	void Write_Raw(const std::string& content)
	{
		Write_Line(content);
	}

	void Write_Result(const std::string& operation, const std::string& result)
	{
		m_Test_Count++;
		m_Test_Passed++;

		std::string prefix = operation;
		std::stringstream ss;
		ss << std::left << std::setw(LABEL_WIDTH) << (prefix + " :") << result;
		Write_Line(ss.str());
	}

	void Write_Property(const std::string& property, bool value)
	{
		m_Test_Count++;
		if (value) m_Test_Passed++;
		else m_Test_Failed++;

		std::string status = value ? "TRUE" : "FALSE";
		std::string prefix = property;

		std::stringstream ss;
		ss << std::left << std::setw(LABEL_WIDTH) << (prefix + " :") << status;
		Write_Line(ss.str());
	}

	void Write_Error(const std::string& message)
	{
		m_Test_Count++; // Increment total operations count for errors too
		m_Test_Failed++;
		Write_Line("\nERROR: " + message + "\n");
	}

	void Write_Summary()
	{
		std::string border(120, '=');
		Write_Line("");
		Write_Line("");
		Write_Line(border);
		Write_Line("  EXECUTION SUMMARY");
		Write_Line(border);

		auto LogStat = [&](std::string label, int val) {
			std::stringstream ss;
			ss << "  " << std::left << std::setw(20) << label << ": " << val;
			Write_Line(ss.str());
			};

		LogStat("Total Operations", m_Test_Count);
		LogStat("Successful", m_Test_Passed);
		LogStat("Failed/False", m_Test_Failed);

		Write_Line(border);
		Write_Line("  End of Report");
	}

private:
	void Write_Line(const std::string& line)
	{
		m_Output_File << line << "\n";
		if (m_Console_Echo)
		{
			std::cout << line << std::endl;
		}
	}
};