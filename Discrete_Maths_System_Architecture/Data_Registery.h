#pragma once

#include "Type_Systems.h"

// std::variant allows a variable to hold different types at runtime
// This is needed for storing sets, relations, graphs, and functions of multiple types in one container
using AnySet = std::variant<
	Math_Set<int>,
	Math_Set<char>,
	Math_Set<std::string>
>;

// AnyRelation can store a relation of int, char, or string type
using AnyRelation = std::variant<
	Relation<int>,
	Relation<char>,
	Relation<std::string>
>;

// AnyGraph can store a graph of int, char, or string type
using AnyGraph = std::variant<
	Graph<int>,
	Graph<char>,
	Graph<std::string>
>;

// AnyFunction supports all combinations of domain and codomain types
// This covers every possible mapping between int, char, and string
using AnyFunction = std::variant<
	Function<int, int>, Function<int, char>, Function<int, std::string>,
	Function<char, int>, Function<char, char>, Function<char, std::string>,
	Function<std::string, int>, Function<std::string, char>, Function<std::string, std::string>
>;

// Data_Registry is the central storage for all objects in the system
// Each object type is stored in a separate unordered_map for fast lookup by name
class Data_Registry
{
public:
	// m_Sets stores all sets, mapped by their string name
	std::unordered_map<std::string, AnySet>      m_Sets;
	// m_Relations stores all relations, mapped by their string name
	std::unordered_map<std::string, AnyRelation> m_Relations;
	// m_Functions stores all functions, mapped by their string name
	std::unordered_map<std::string, AnyFunction> m_Functions;
	// m_Graphs stores all graphs, mapped by their string name
	std::unordered_map<std::string, AnyGraph>    m_Graphs;

	// Get_Set_Type returns the type of a set stored in the registry
	// std::holds_alternative checks which type the variant currently holds
	// This is important for type safety and for dispatching correct operations
	Data_Type Get_Set_Type( const std::string & name ) const
	{
		// If the set does not exist, return UNKNOWN
		if ( m_Sets.find( name ) == m_Sets.end() ) return Data_Type::UNKNOWN;

		const auto & s = m_Sets.at( name ); // get the variant for the set
		if ( std::holds_alternative<Math_Set<int>>( s ) ) return Data_Type::INT; // check for int set
		if ( std::holds_alternative<Math_Set<char>>( s ) ) return Data_Type::CHAR; // check for char set
		if ( std::holds_alternative<Math_Set<std::string>>( s ) ) return Data_Type::STRING; // check for string set

		return Data_Type::UNKNOWN; // fallback if type not recognized
	}

	// Exists_Set checks if a set with the given name exists in the registry
	// count is used for fast lookup, returns true if found
	bool Exists_Set( const std::string & name ) const
	{
		return m_Sets.count( name );
	}

	// Exists_Relation checks if a relation with the given name exists
	bool Exists_Relation( const std::string & name ) const
	{
		return m_Relations.count( name );
	}

	// Exists_Function checks if a function with the given name exists
	bool Exists_Function( const std::string & name ) const
	{
		return m_Functions.count( name );
	}

	// Exists_Graph checks if a graph with the given name exists
	bool Exists_Graph( const std::string & name ) const
	{
		return m_Graphs.count( name );
	}
};