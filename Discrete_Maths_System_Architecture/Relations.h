#pragma once

#include "Graph_Utility.h"

// This class models a binary relation on a set
// Uses Math_Set for domain and for storing ordered pairs
template <Comparable type>
class Relation
{
private:
	Math_Set<type> m_Relation_Domain; // stores the domain of the relation
	Math_Set<std::pair<type, type>> m_Relation_Order_Pairs; // stores the ordered pairs

	// Finds the index of an element in the domain
	int Get_Element_Index( const type & element ) const
	{
		int size = m_Relation_Domain.Get_Size();
		for ( int i = 0; i < size; i++ )
		{
			if ( m_Relation_Domain [ i ] == element )
				return i; // return index if found
		}
		return -1; // return -1 if not found
	}

	// Builds an adjacency matrix for the relation
	std::vector<std::vector<bool>> Build_Adjacency_Matrix() const
	{
		int size = m_Relation_Domain.Get_Size();
		std::vector<std::vector<bool>> matrix( size, std::vector<bool>( size, false ) );

		int pairs_size = m_Relation_Order_Pairs.Get_Size();
		for ( int p = 0; p < pairs_size; p++ )
		{
			std::pair<type, type> pair = m_Relation_Order_Pairs [ p ];
			int i = Get_Element_Index( pair.first );
			int j = Get_Element_Index( pair.second );

			if ( i != -1 && j != -1 )
				matrix [ i ][ j ] = true; // mark relation in matrix
		}

		return matrix;
	}

public:
	// Constructor sets the domain
	Relation( const Math_Set<type> & domain )
	{
		m_Relation_Domain = domain;
	}

	// Copy constructor copies domain and pairs
	Relation( const Relation<type> & other )
	{
		m_Relation_Domain = other.m_Relation_Domain;
		m_Relation_Order_Pairs = other.m_Relation_Order_Pairs;
	}

	Relation()
	{
	}

   // Adds a pair to the relation if both elements are in the domain
	void Add_Pair( type element_a, type element_b )
	{
		int pos_a = m_Relation_Domain.Binary_Search_Position( element_a );
		int pos_b = m_Relation_Domain.Binary_Search_Position( element_b );

		// Only add if both elements are present in the domain
		if ( !( pos_a < m_Relation_Domain.Get_Size() && m_Relation_Domain [ pos_a ] == element_a ) ||
			 !( pos_b < m_Relation_Domain.Get_Size() && m_Relation_Domain [ pos_b ] == element_b ) )
			return;

		m_Relation_Order_Pairs.Insert_Element( { element_a, element_b } );
	}

	// Removes a pair from the relation
	void Remove_Pair( type element_a, type element_b )
	{
		m_Relation_Order_Pairs.Remove_Element( { element_a, element_b } );
	}

	// Checks if a pair exists in the relation
	bool Contains_Pair( type element_a, type element_b ) const
	{
		int pos = m_Relation_Order_Pairs.Binary_Search_Position( { element_a, element_b } );
		if ( pos < m_Relation_Order_Pairs.Get_Size() && m_Relation_Order_Pairs [ pos ] == std::pair<type, type>{ element_a, element_b } )
			return true;

		return false;
	}

	// Checks if the relation is reflexive
	// Every element must be related to itself
	bool Is_Reflexive() const
	{
		int size = m_Relation_Domain.Get_Size();

		for ( int i = 0; i < size; i++ )
		{
			type element = m_Relation_Domain [ i ];

			if ( !m_Relation_Order_Pairs.Contains_Element( { element, element } ) )
				return false; // missing reflexive pair
		}
		return true;
	}

	// Checks if the relation is symmetric
	// For every (a,b), (b,a) must also be present
	bool Is_Symmetric() const
	{
		int size = m_Relation_Order_Pairs.Get_Size();

		for ( int i = 0; i < size; i++ )
		{
			std::pair<type, type> element = m_Relation_Order_Pairs [ i ];

			if ( !m_Relation_Order_Pairs.Contains_Element( { element.second, element.first } ) )
				return false; // missing symmetric pair
		}
		return true;
	}

	// Checks if the relation is antisymmetric
	// For every (a,b) and (b,a), a must equal b
	bool Is_AntiSymmetric() const
	{
		int size = m_Relation_Order_Pairs.Get_Size();

		for ( int i = 0; i < size; i++ )
		{
			std::pair<type, type> element = m_Relation_Order_Pairs [ i ];

			if ( m_Relation_Order_Pairs.Contains_Element( { element.second, element.first } ) && element.first != element.second )
				return false; // found non-antisymmetric pair
		}
		return true;
	}

	// Checks if the relation is transitive
	// For every (a,b) and (b,c), (a,c) must be present
	bool Is_Transitive() const
	{
		int size = m_Relation_Order_Pairs.Get_Size();

		for ( int i = 0; i < size; i++ )
		{
			std::pair<type, type> first_pair = m_Relation_Order_Pairs [ i ];

			for ( int j = 0; j < size; j++ )
			{
				if ( i == j )
					continue;
				std::pair<type, type> second_pair = m_Relation_Order_Pairs [ j ];

				if ( second_pair.first == first_pair.second &&
					 !m_Relation_Order_Pairs.Contains_Element( { first_pair.first, second_pair.second } ) )
					return false; // missing transitive pair
			}
		}
		return true;
	}

	// Checks if the relation is an equivalence relation
	// Must be reflexive, symmetric, and transitive
	bool Is_Equivalence_Relation() const
	{
		return Is_Reflexive() && Is_Symmetric() && Is_Transitive();
	}

	// Checks if the relation is a partial order
	// Must be reflexive, antisymmetric, and transitive
	bool Is_Partial_Order() const
	{
		return Is_Reflexive() && Is_AntiSymmetric() && Is_Transitive();
	}

	// Returns the reflexive closure of the relation
	// Adds missing (a,a) pairs
	Relation<type> Reflexive_Closure() const
	{
		Relation <type> Reflexive_Closure = *this;

		int size = m_Relation_Domain.Get_Size();

		for ( int i = 0; i < size; i++ )
		{
			type element = m_Relation_Domain [ i ];

			if ( !m_Relation_Order_Pairs.Contains_Element( { element, element } ) )
				Reflexive_Closure.Add_Pair( element, element );
		}

		return Reflexive_Closure;
	}

	// Returns the symmetric closure of the relation
	// Adds missing (b,a) pairs for every (a,b)
	Relation<type> Symmetric_Closure() const
	{
		Relation<type> Symmetric_Closure = *this;

		int size = m_Relation_Order_Pairs.Get_Size();

		for ( int i = 0; i < size; i++ )
		{
			std::pair<type, type> element = m_Relation_Order_Pairs [ i ];

			if ( !m_Relation_Order_Pairs.Contains_Element( { element.second, element.first } ) )
				Symmetric_Closure.Add_Pair( element.second, element.first );
		}

		return Symmetric_Closure;
	}

	// Returns the transitive closure of the relation
	// Uses Warshall's algorithm to add missing transitive pairs
	Relation<type> Transitive_Closure() const
	{
		Relation<type> Transitive_Closure = *this;

		std::vector<std::vector<bool>> matrix = Build_Adjacency_Matrix();
		int n = m_Relation_Domain.Get_Size();

		for ( int k = 0; k < n; k++ )
		{
			for ( int i = 0; i < n; i++ )
			{
				for ( int j = 0; j < n; j++ )
				{
					if ( matrix [ i ][ k ] && matrix [ k ][ j ] )
					{
						matrix [ i ][ j ] = true; // add transitive connection
					}
				}
			}
		}

		for ( int i = 0; i < n; i++ )
		{
			for ( int j = 0; j < n; j++ )
			{
				if ( matrix [ i ][ j ] )
				{
					Transitive_Closure.Add_Pair( m_Relation_Domain [ i ], m_Relation_Domain [ j ] );
				}
			}
		}

		return Transitive_Closure;
	}

	// Prints all relation pairs in the relation
	void Print_Relation_Pairs() const
	{
		std::cout << "{ ";
		int size = m_Relation_Order_Pairs.Get_Size();
		for ( int i = 0; i < size; ++i )
		{
			const auto & pair = m_Relation_Order_Pairs [ i ];
			std::cout << "(" << pair.first << "," << pair.second << ")";
			if ( i < size - 1 )
				std::cout << ", ";
		}
		std::cout << " }" << std::endl;
	}

	// Returns the size of the domain
	int Get_Domain_Size() const
	{
		return m_Relation_Domain.Get_Size();
	}

	// Returns the number of pairs in the relation
	int Get_Relation_Size() const
	{
		return m_Relation_Order_Pairs.Get_Size();
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Relation<type>& rel)
	{
		os << rel.m_Relation_Order_Pairs; // Reuses Math_Set's operator
		return os;
	}
};