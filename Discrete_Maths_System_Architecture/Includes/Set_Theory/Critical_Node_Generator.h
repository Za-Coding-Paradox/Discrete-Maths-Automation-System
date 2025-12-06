#pragma once

#include "Quantifier_Validator.h"

// This concept restricts the template type to int, char, or string
// Using C++20 concepts and type traits for compile-time type checking
template <typename type>
concept Allowed_Node_Type =
(
	std::same_as<type, int> ||
	std::same_as<type, char> ||
	std::same_as<type, std::string>
	);

// This class provides static functions for generating and validating node sets
// All functions are static so you do not need to create an object
template <Allowed_Node_Type type>
class Critical_Node_Generator
{
public:
	Critical_Node_Generator() = delete; // disables object creation

	// Combines high value nodes and frequently accessed nodes using intersection
	// Then adds gateway nodes using union
	static Math_Set<type> Generate_Priority_Nodes(
		const Math_Set<type> & high_Value_Nodes,
		const Math_Set<type> & frequently_Accessed_Nodes,
		const Math_Set<type> & gateway_Nodes )
	{
		// First get nodes that are both high value and frequently accessed
		Math_Set<type> must_Include_Nodes = INTERSECTION( high_Value_Nodes, frequently_Accessed_Nodes );
		// Add gateway nodes to the set
		must_Include_Nodes = UNION( must_Include_Nodes, gateway_Nodes );

		return must_Include_Nodes;
	}

	// Generates the set of nodes that should be excluded
	// Removes must-include nodes from the mainatainence nodes
	static Math_Set<type> Generate_Excluded_Nodes(
		const Math_Set<type> & total_Nodes,
		const Math_Set<type> & must_Include_Nodes,
		const Math_Set<type> & mainatainence_Nodes )
	{
		// Returns all nodes in mainatainence_Nodes that are not in must_Include_Nodes
		return DIFFERENCE( mainatainence_Nodes, must_Include_Nodes );
	}

	// Removes priority nodes from the total set
	static Math_Set<type> Generate_Optional_Nodes(
		const Math_Set<type> & total_Nodes,
		const Math_Set<type> & priority_Nodes )
	{
		return DIFFERENCE( total_Nodes, priority_Nodes );
	}

	// A valid partition has no overlap between sets and covers the universal set
	// Uses vector for partitions and set operations for checking
	static bool Validate_Partition(
		const Math_Set<type> & universal_Set,
		const std::vector<Math_Set<type>> & parititions )
	{
		int size = parititions.size();

		// Start with the first partition
		Math_Set<type> union_set = parititions [ 0 ];
		Math_Set<type> intersection_set;

		// Check for overlap between consecutive partitions
		for ( int i = 1; i < size; i++ )
		{
			intersection_set = INTERSECTION( parititions [ i - 1 ], parititions [ i ] );
			if ( !intersection_set.Is_Empty() )
				return false;

			// Add current partition to the union set
			union_set = UNION( union_set, parititions [ i ] );
		}

		// Check if union of all partitions equals the universal set
		if ( union_set != universal_Set )
			return false;

		return true;
	}
};