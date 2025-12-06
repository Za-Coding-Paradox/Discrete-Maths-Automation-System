#pragma once

#include "Relations.h"

// This class models a mathematical function from type_A to type_B
// Uses Math_Set for domain and codomain, and unordered_map for mappings
template <Comparable type_A, Comparable type_B>
class Function
{
private:
	Math_Set<type_A> m_Function_Domain; // stores the domain of the function
	Math_Set<type_B> m_Function_CoDomain; // stores the codomain of the function

	// maps each input to an optional output, allows for undefined mappings
	std::unordered_map<type_A, std::optional<type_B>> m_Function_Mapping;

public:
	// Constructor sets domain and codomain
	Function( const Math_Set<type_A> & Domain, const Math_Set<type_B> & CoDomain )
	{
		m_Function_Domain = Domain;
		m_Function_CoDomain = CoDomain;
	}

	// Default constructor, useful for managing variant's in the Data Registry
	Function()
	{
	}

   // Returns reference to the domain set
	const Math_Set<type_A> & Get_Domain() const
	{
		return m_Function_Domain;
	}

	// Returns reference to the codomain set
	const Math_Set<type_B> & Get_CoDomain() const
	{
		return m_Function_CoDomain;
	}

	// Returns the size of the domain
	int Get_Domain_Size() const
	{
		return m_Function_Domain.Get_Size();
	}

	// Returns the size of the codomain
	int Get_CoDomain_Size() const
	{
		return m_Function_CoDomain.Get_Size();
	}

	// Defines a mapping from input to output if both are in domain and codomain
	void Define_Mapping( type_A input, type_B output )
	{
		if ( !m_Function_Domain.Contains_Element( input ) || !m_Function_CoDomain.Contains_Element( output ) )
			return; // do nothing if input or output not valid

		m_Function_Mapping [ input ] = output; // set mapping
	}

	// Evaluates the function at a given input
	// Returns optional output, empty if not defined
	std::optional<type_B> Evaluate_Function( type_A input ) const
	{
		if ( !Is_Defined_At( input ) )
			return std::nullopt; // return empty if not defined

		return m_Function_Mapping.at( input ).value(); // return mapped value
	}

	// Checks if a mapping is defined for the input
	bool Is_Defined_At( type_A input ) const
	{
		if ( m_Function_Mapping.count( input ) && m_Function_Mapping.at( input ).has_value() )
			return true; // mapping exists and is not empty
		return false;
	}

	// Checks if the function is proper, meaning every domain element has a mapping
	bool Is_Proper_Function() const
	{
		int size = m_Function_Domain.Get_Size();

		for ( int i = 0; i < size; i++ )
		{
			const type_A & input = m_Function_Domain [ i ];

			if ( !Is_Defined_At( input ) )
				return false; // found domain element with no mapping
		}

		return true;
	}

	// Checks if the function is injective
	// Each output is mapped from at most one input
	bool Is_Injective() const
	{
		if ( !Is_Proper_Function() )
			return false; // must be proper first

		std::unordered_map<type_B, int> Co_Domain_Frequency; // counts how many times each output is used
		int size = m_Function_CoDomain.Get_Size();

		for ( int i = 0; i < size; i++ )
		{
			Co_Domain_Frequency [ m_Function_CoDomain [ i ] ] = 0; // initialize frequency
		}

		size = m_Function_Domain.Get_Size();

		for ( int i = 0; i < size; i++ )
		{
			const type_A & input = m_Function_Domain [ i ];

			const auto output = Evaluate_Function( input );

			if ( !output.has_value() || ++Co_Domain_Frequency [ output.value() ] == 2 )
				return false; // output used more than once or not defined
		}

		return true;
	}

	// Checks if the function is surjective
	// Every codomain element is mapped to by at least one domain element
	bool Is_Surjective() const
	{
		if ( !Is_Proper_Function() )
			return false; // must be proper first

		std::unordered_map<type_B, int> Co_Domain_Frequency; // counts how many times each output is used
		int size = m_Function_CoDomain.Get_Size();

		for ( int i = 0; i < size; i++ )
		{
			Co_Domain_Frequency [ m_Function_CoDomain [ i ] ] = 0; // initialize frequency
		}

		size = m_Function_Domain.Get_Size();

		for ( int i = 0; i < size; i++ )
		{
			const type_A & input = m_Function_Domain [ i ];

			const auto output = Evaluate_Function( input );

			if ( output.has_value() )
				Co_Domain_Frequency [ output.value() ]++; // increment count for output
		}

		for ( const auto & [key, value] : Co_Domain_Frequency )
			if ( value == 0 )
				return false; // found codomain element not mapped to

		return true;
	}

	// Checks if the function is bijective
	// Must be both injective and surjective
	bool Is_Bijective() const
	{
		return Is_Injective() && Is_Surjective();
	}

	// Composes this function with another function
	// Returns a new function representing the composition
	template <Comparable type_C>
	Function<type_A, type_C> Define_Composition( const Function<type_B, type_C> & other_function ) const
	{
		Function<type_A, type_C> composition( m_Function_Domain, other_function.Get_CoDomain() );

		int size = m_Function_Domain.Get_Size();
		for ( int i = 0; i < size; i++ )
		{
			const type_A & input = m_Function_Domain [ i ];

			if ( Is_Defined_At( input ) )
			{
				auto intermediate = Evaluate_Function( input );
				if ( intermediate.has_value() )
				{
					auto final_output = other_function.Evaluate_Function( intermediate.value() );
					if ( final_output.has_value() )
					{
						composition.Define_Mapping( input, final_output.value() );
					}
				}
			}
		}

		return composition;
	}

	// Returns the inverse function if bijective, otherwise returns empty optional
	std::optional<Function<type_B, type_A>> Define_Inverse() const
	{
		if ( !Is_Bijective() )
			return std::nullopt; // cannot invert if not bijective

		Function<type_B, type_A> inverse( m_Function_CoDomain, m_Function_Domain );

		int size = m_Function_Domain.Get_Size();
		for ( int i = 0; i < size; i++ )
		{
			const type_A & input = m_Function_Domain [ i ];

			if ( Is_Defined_At( input ) )
			{
				auto output = Evaluate_Function( input );
				if ( output.has_value() )
				{
					inverse.Define_Mapping( output.value(), input ); // reverse mapping
				}
			}
		}

		return inverse;
	}

	// Prints all images of the function
	void Print_Images() const
	{
		std::cout << "{ ";
		Math_Set<type_B> images = Get_Images();
		for ( int i = 0; i < images.Get_Size(); ++i )
		{
			std::cout << images [ i ];
			if ( i < images.Get_Size() - 1 )
				std::cout << ", ";
		}
		std::cout << " }" << std::endl;
	}

	// Prints all preimages of the function
	void Print_PreImages() const
	{
		std::cout << "{ ";
		Math_Set<type_A> preimages = Get_PreImages();
		for ( int i = 0; i < preimages.Get_Size(); ++i )
		{
			std::cout << preimages [ i ];
			if ( i < preimages.Get_Size() - 1 )
				std::cout << ", ";
		}
		std::cout << " }" << std::endl;
	}

	// Returns all images, that is, all outputs that are actually mapped to
	Math_Set<type_B> Get_Images() const
	{
		Math_Set<type_B> images;

		int size = m_Function_Domain.Get_Size();
		for ( int i = 0; i < size; i++ )
		{
			const type_A & input = m_Function_Domain [ i ];

			if ( Is_Defined_At( input ) )
			{
				auto output = Evaluate_Function( input );
				if ( output.has_value() )
				{
					images.Insert_Element( output.value() );
				}
			}
		}

		return images;
	}

	// Returns all preimages, that is, all domain elements that have mappings
	Math_Set<type_A> Get_PreImages() const
	{
		Math_Set<type_A> preimages;

		int size = m_Function_Domain.Get_Size();
		for ( int i = 0; i < size; i++ )
		{
			const type_A & input = m_Function_Domain [ i ];

			if ( Is_Defined_At( input ) )
			{
				preimages.Insert_Element( input );
			}
		}

		return preimages;
	}

	friend std::ostream& operator<<(std::ostream& os, const Function<type_A, type_B>& func)
	{
		os << "Map [Size: " << func.m_Function_Domain.Get_Size() << "]: ";
		bool first = true;
		os << "{ ";
		for (const auto& [input, output_opt] : func.m_Function_Mapping)
		{
			if (output_opt.has_value())
			{
				if (!first) os << ", ";
				os << input << " -> " << output_opt.value();
				first = false;
			}
		}
		os << " }";
		return os;
	}
};