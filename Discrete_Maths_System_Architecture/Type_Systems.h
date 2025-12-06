#pragma once

#include "Functions.h"

// This enum class is used to represent the type of data at runtime
// Using enum class gives better type safety and avoids name clashes
enum class Data_Type
{
	INT,        // for integer values
	CHAR,       // for character values
	STRING,     // for string values
	UNKNOWN     // for any type not recognized
};

// This struct holds information about a type
// It is useful for runtime type management and conversions
struct Type_Info
{
	Data_Type m_Data_Type;      // stores the actual type as an enum
	std::string m_Type_Name;    // stores the type name as a string

	// Converts a string to a Data_Type value
	// Using std::transform and ::tolower to make the comparison case-insensitive
	static Data_Type From_String( const std::string & str )
	{
		std::string lower_str = str; // make a copy so I can modify it
		std::transform( lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower ); // convert to lowercase

		if ( lower_str == "int" || lower_str == "integer" )
			return Data_Type::INT; // match integer types
		if ( lower_str == "char" || lower_str == "character" )
			return Data_Type::CHAR; // match character types
		if ( lower_str == "string" || lower_str == "str" )
			return Data_Type::STRING; // match string types

		return Data_Type::UNKNOWN; // fallback for unknown types
	}

	// Converts a Data_Type value to its string representation
	// Using a switch statement for clarity and efficiency
	static std::string To_String( Data_Type type )
	{
		switch ( type )
		{
			case Data_Type::INT: return "int";
			case Data_Type::CHAR: return "char";
			case Data_Type::STRING: return "string";
			default: return "unknown";
		}
	}

	// Checks if the type is valid (not UNKNOWN)
	static bool Is_Valid( Data_Type type )
	{
		return type != Data_Type::UNKNOWN;
	}
};

// This class provides static functions for converting between strings and types
// Using templates so it works for any comparable type
class Type_Converter
{
public:
	Type_Converter() = delete; // Delete the constructor to prevent instantiation

	// Converts a string to a value of the given type
	// Using if constexpr so the code is checked at compile time for each type
	template <Comparable type>
	static type From_String( const std::string & str )
	{
		if constexpr ( std::is_same_v<type, int> )
		{
			return std::stoi( str ); // convert string to int
		}
		else if constexpr ( std::is_same_v<type, char> )
		{
			return str.empty() ? '\0' : str [ 0 ]; // get first character or null if empty
		}
		else if constexpr ( std::is_same_v<type, std::string> )
		{
			return str; // already a string, just return
		}
		else
		{
			// If the type is not supported, throw an exception
			throw std::runtime_error( "Unsupported type for conversion" );
		}
	}

	// Converts a value to its string representation
	// Using if constexpr for type-specific logic
	template <Comparable type>
	static std::string To_String( const type & value )
	{
		if constexpr ( std::is_same_v<type, int> )
		{
			return std::to_string( value ); // convert int to string
		}
		else if constexpr ( std::is_same_v<type, char> )
		{
			return std::string( 1, value ); // convert char to string
		}
		else if constexpr ( std::is_same_v<type, std::string> )
		{
			return value; // already a string
		}
		else
		{
			// For other types, use stringstream for conversion
			std::ostringstream oss;
			oss << value;
			return oss.str();
		}
	}

	// Parses a string and trims whitespace before conversion
	// This is important for user input and file parsing
	template <Comparable type>
	static type Parse_Element( const std::string & str )
	{
		std::string trimmed = Trim( str ); // remove leading and trailing whitespace

		// For string types, remove quotes if present
		if constexpr ( std::is_same_v<type, std::string> )
		{
			if ( trimmed.length() >= 2 &&
				 ( ( trimmed.front() == '"' && trimmed.back() == '"' ) ||
				   ( trimmed.front() == '\'' && trimmed.back() == '\'' ) ) )
			{
				trimmed = trimmed.substr( 1, trimmed.length() - 2 ); // remove quotes
			}
		}

		return From_String<type>( trimmed ); // convert to the correct type
	}

private:
	// Removes whitespace from both ends of a string
	// This is useful for cleaning up user input and file data
	static std::string Trim( const std::string & str )
	{
		auto start = str.find_first_not_of( " \t\n\r" ); // find first non-whitespace
		auto end = str.find_last_not_of( " \t\n\r" ); // find last non-whitespace
		return ( start == std::string::npos ) ? "" : str.substr( start, end - start + 1 );
	}
};

// This utility class provides static functions for string manipulation
// I use static functions so I do not need to create an object
class String_Utils
{
public:
	String_Utils() = delete; // disables object creation

	// Removes whitespace from both ends of a string
	static std::string Trim( const std::string & str )
	{
		auto start = str.find_first_not_of( " \t\n\r" );
		auto end = str.find_last_not_of( " \t\n\r" );
		return ( start == std::string::npos ) ? "" : str.substr( start, end - start + 1 );
	}

	// Splits a string into tokens using a delimiter
	// I use stringstream and getline for efficient splitting
	static std::vector<std::string> Split( const std::string & str, char delimiter )
	{
		std::vector<std::string> tokens;
		std::stringstream ss( str );
		std::string token;

		while ( std::getline( ss, token, delimiter ) )
		{
			tokens.push_back( Trim( token ) ); // trim each token before adding
		}

		return tokens;
	}

	// Converts a string to uppercase
	// Using std::transform and ::toupper for character conversion
	static std::string To_Upper( const std::string & str )
	{
		std::string result = str;
		std::transform( result.begin(), result.end(), result.begin(), ::toupper );
		return result;
	}

	// Converts a string to lowercase
	static std::string To_Lower( const std::string & str )
	{
		std::string result = str;
		std::transform( result.begin(), result.end(), result.begin(), ::tolower );
		return result;
	}

	// Checks if a string starts with a given prefix
	// Useful for parsing commands and configuration
	static bool Starts_With( const std::string & str, const std::string & prefix )
	{
		return str.size() >= prefix.size() &&
			str.compare( 0, prefix.size(), prefix ) == 0;
	}

	// Checks if a string contains a substring
	static bool Contains( const std::string & str, const std::string & substr )
	{
		return str.find( substr ) != std::string::npos;
	}

	// Removes all occurrences of a character from a string
	static std::string Remove_Char( const std::string & str, char ch )
	{
		std::string result = str;
		result.erase( std::remove( result.begin(), result.end(), ch ), result.end() );
		return result;
	}

	// Replaces all occurrences of a substring with another substring
	// Using a loop to find and replace each occurrence
	static std::string Replace( const std::string & str, const std::string & from, const std::string & to )
	{
		std::string result = str;
		size_t pos = 0;

		while ( ( pos = result.find( from, pos ) ) != std::string::npos )
		{
			result.replace( pos, from.length(), to );
			pos += to.length();
		}

		return result;
	}
};