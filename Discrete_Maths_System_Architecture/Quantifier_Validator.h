#pragma once

#include "Math_Set.h"

// static class for validating quantifiers on a single set
// template ensures it works for any type (int, string, etc)
template <Comparable type>
class Single_Quantifier_Validator
{
public:
	// disable construction since we only use static methods
	Single_Quantifier_Validator() = delete;

	// universal quantifier means for all x in set p(x) is true
	// we use 'auto' for the predicate to accept lambdas easily
	static bool Universal_Quantifier(const Math_Set<type>& Domain, const auto& Predicate)
	{
		const int size = Domain.Get_Size();
		for (int i = 0; i < size; i++)
		{
			// if we find even one failure return false immediately
			if (!Predicate(Domain[i]))
				return false;
		}
		return true;
	}

	// existential quantifier means there exists an x such that p(x) is true
	static bool Existential_Quantifier(const Math_Set<type>& Domain, const auto& Predicate)
	{
		const int size = Domain.Get_Size();
		for (int i = 0; i < size; i++)
		{
			// if we find one success return true immediately
			if (Predicate(Domain[i]))
				return true;
		}
		return false;
	}

	// unique existential means there is exactly one x
	static bool Unique_Existential_Quantifier(const Math_Set<type>& Domain, const auto& Predicate)
	{
		const int size = Domain.Get_Size();
		bool found_one = false;

		for (int i = 0; i < size; i++)
		{
			if (Predicate(Domain[i]))
			{
				// if we already found one and find another fail
				if (found_one)
					return false;
				found_one = true;
			}
		}

		return found_one;
	}

	// demorgan law for universal
	// not (for all x p(x)) is equivalent to there exists x such that not p(x)
	// basically checking if "not all" pass
	static bool Universal_Demorgan(const Math_Set<type>& Domain, const auto& Predicate)
	{
		return !Universal_Quantifier(Domain, Predicate);
	}

	// demorgan law for existential
	// not (exists x p(x)) is equivalent to for all x not p(x)
	// basically checking if "none" pass
	static bool Existential_Demorgan(const Math_Set<type>& Domain, const auto& Predicate)
	{
		return !Existential_Quantifier(Domain, Predicate);
	}
};

// handles nested quantifiers like for all x exists y
template <Comparable type_A, Comparable type_B>
class Nested_Quantifier_Validator
{
public:
	Nested_Quantifier_Validator() = delete;

	// for all x in a and for all y in b
	static bool Nested_Quantifier_Universal_Universal(
		const Math_Set<type_A>& Domain_A,
		const Math_Set<type_B>& Domain_B,
		const auto& Predicate)
	{
		const int size_A = Domain_A.Get_Size();
		const int size_B = Domain_B.Get_Size();

		// nested loops to check every combination
		for (int i = 0; i < size_A; i++)
		{
			for (int j = 0; j < size_B; j++)
			{
				if (!Predicate(Domain_A[i], Domain_B[j]))
					return false;
			}
		}

		return true;
	}

	// for all x in a there exists a y in b
	static bool Nested_Quantifier_Universal_Existential(
		const Math_Set<type_A>& Domain_A,
		const Math_Set<type_B>& Domain_B,
		const auto& Predicate)
	{
		const int size_A = Domain_A.Get_Size();
		const int size_B = Domain_B.Get_Size();

		for (int i = 0; i < size_A; i++)
		{
			bool found = false;

			for (int j = 0; j < size_B; j++)
			{
				if (Predicate(Domain_A[i], Domain_B[j]))
				{
					found = true;
					break;
				}
			}

			// if we finished inner loop without finding a y fail
			if (!found)
				return false;
		}

		return true;
	}

	// there exists x in a and there exists y in b
	static bool Nested_Quantifier_Existential_Existential(
		const Math_Set<type_A>& Domain_A,
		const Math_Set<type_B>& Domain_B,
		const auto& Predicate)
	{
		const int size_A = Domain_A.Get_Size();
		const int size_B = Domain_B.Get_Size();

		for (int i = 0; i < size_A; i++)
		{
			for (int j = 0; j < size_B; j++)
			{
				// just need one pair to work
				if (Predicate(Domain_A[i], Domain_B[j]))
					return true;
			}
		}

		return false;
	}

	// there exists x in a such that for all y in b
	static bool Nested_Quantifier_Existential_Universal(
		const Math_Set<type_A>& Domain_A,
		const Math_Set<type_B>& Domain_B,
		const auto& Predicate)
	{
		const int size_A = Domain_A.Get_Size();
		const int size_B = Domain_B.Get_Size();

		for (int i = 0; i < size_A; i++)
		{
			bool all_true = true;

			for (int j = 0; j < size_B; j++)
			{
				if (!Predicate(Domain_A[i], Domain_B[j]))
				{
					all_true = false;
					break;
				}
			}

			// if inner loop finished with all_true still true we found our x
			if (all_true)
				return true;
		}

		return false;
	}
};