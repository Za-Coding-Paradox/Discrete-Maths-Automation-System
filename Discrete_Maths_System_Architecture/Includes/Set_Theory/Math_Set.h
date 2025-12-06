#pragma once

#include "Core.h"


// c++20 concept definition
// this enforces that any type used in our sets must support comparison
// this is crucial because our sets are sorted and require <, >, ==
template<typename type>
concept Comparable = requires(type a, type b)
{
	{ a < b } -> std::convertible_to<bool>;
	{ a == b } -> std::convertible_to<bool>;
	{ a > b } -> std::convertible_to<bool>;
};

// global overloading of << operator for pairs
// this is needed because cartesian products create sets of pairs
// and we need to be able to print them easily
template <Comparable type_A, Comparable type_B>
std::ostream& operator<<(std::ostream& os, const std::pair<type_A, type_B>& p)
{
	os << "( " << p.first << ", " << p.second << " )";
	return os;
}

// our custom set class
// constrained by the comparable concept
template <Comparable type>
class Math_Set
{
private:
	// using unique_ptr for automatic memory management
	// when the set is destroyed, this array is automatically deleted
	std::unique_ptr<type[]> m_Data_Set;
	int m_Capacity;
	int m_Size;

	// declaring these operations as friends so they can access private data
	// this allows them to be implemented efficiently without getters
	template<Comparable U>
	friend Math_Set<U> UNION(const Math_Set<U>& Set1, const Math_Set<U>& Set2);

	template<Comparable U>
	friend Math_Set<U> INTERSECTION(const Math_Set<U>& Set1, const Math_Set<U>& Set2);

	template<Comparable U>
	friend Math_Set<U> DIFFERENCE(const Math_Set<U>& Set1, const Math_Set<U>& Set2);

	template<Comparable U>
	friend Math_Set<U> SYMMETRIC_DIFFERENCE(const Math_Set<U>& Set1, const Math_Set<U>& Set2);

	template<Comparable A, Comparable B>
	friend Math_Set<std::pair<A, B>> CARTESIAN_PRODUCT(const Math_Set<A>& Set1, const Math_Set<B>& Set2);

	template<Comparable U>
	friend Math_Set<Math_Set<U>> POWER_SET(const Math_Set<U>&);

	template<Comparable U>
	friend bool IS_SUBSET(const Math_Set<U>& Set1, const Math_Set<U>& Set2);

	template<Comparable U>
	friend bool IS_PROPER_SUBSET(const Math_Set<U>& Set1, const Math_Set<U>& Set2);

	// stream insertion for easy printing
	friend std::ostream& operator<<(std::ostream& os, const Math_Set<type>& set)
	{
		os << "{";
		for (int i = 0; i < set.m_Size; i++)
		{
			os << set.m_Data_Set[i];
			if (i < set.m_Size - 1)
				os << ",";
		}
		os << "}";
		return os;
	}

	// stream extraction for reading sets from input
	friend std::istream& operator>>(std::istream& is, Math_Set<type>& s)
	{
		s.Clear_Set();
		char ch;
		is >> ch; // read opening brace

		type element;
		while (is >> element)
		{
			s.Insert_Element(element);
			is >> ch; // read comma or closing brace
			if (ch == '}')
				break;
		}
		return is;
	}

public:
	// constructor with default capacity
	Math_Set(int capacity = 5)
		: m_Capacity(capacity > 0 ? capacity : 5)
		, m_Size(0)
	{
		// allocate raw memory managed by unique_ptr
		m_Data_Set = std::make_unique<type[]>(m_Capacity);
	}

	// copy constructor
	// deep copy is required because unique_ptr cannot be copied
	Math_Set(const Math_Set& other)
		: m_Capacity(other.m_Capacity)
		, m_Size(other.m_Size)
	{
		m_Data_Set = std::make_unique<type[]>(m_Capacity);
		// copy data from source to new memory
		std::copy(other.m_Data_Set.get(),
			other.m_Data_Set.get() + m_Size,
			m_Data_Set.get());
	}

	// construct from raw array
	Math_Set(const type* elements, int count)
		: m_Capacity(count > 5 ? count : 5), m_Size(0)
	{
		m_Data_Set = std::make_unique<type[]>(m_Capacity);
		for (int i = 0; i < count; i++)
		{
			Insert_Element(elements[i]);
		}
	}

	// inserts an element maintaining sorted order
	// sorted order makes operations like union and intersection much faster
	bool Insert_Element(const type& element_To_Insert)
	{
		// double capacity if full
		if (m_Size >= m_Capacity)
		{
			Resize_Set();
		}

		// find correct position via binary search
		int pos = Binary_Search_Position(element_To_Insert);

		// check for duplicates
		if (pos < m_Size&& m_Data_Set[pos] == element_To_Insert)
		{
			return false;
		}

		// shift elements right to make hole
		for (int j = m_Size; j > pos; j--)
		{
			m_Data_Set[j] = m_Data_Set[j - 1];
		}

		// insert new element
		m_Data_Set[pos] = element_To_Insert;
		m_Size++;

		return true;
	}

	bool Remove_Element(const type& element_To_Delete)
	{
		if (m_Size == 0) return false;

		int pos = Binary_Search_Position(element_To_Delete);

		// validate existence
		if (pos >= m_Size || m_Data_Set[pos] != element_To_Delete)
		{
			return false;
		}

		// shift elements left to fill hole
		for (int i = pos; i < m_Size - 1; i++)
		{
			m_Data_Set[i] = m_Data_Set[i + 1];
		}

		m_Size--;
		return true;
	}

	// standard binary search to find element or insertion point
	// complexity is O(log n)
	int Binary_Search_Position(const type& element) const
	{
		int left = 0;
		int right = m_Size;

		while (left < right)
		{
			int mid = left + (right - left) / 2;
			if (m_Data_Set[mid] < element)
				left = mid + 1;
			else
				right = mid;
		}
		return left;
	}

	bool Contains_Element(const type& element) const
	{
		int pos = Binary_Search_Position(element);
		if (pos < m_Size && m_Data_Set[pos] == element)
			return true;
		return false;
	}

	// manual resize logic
	void Resize_Set()
	{
		m_Capacity = (m_Capacity == 0) ? 5 : m_Capacity * 2;
		// create new larger buffer
		std::unique_ptr<type[]> temp = std::make_unique<type[]>(m_Capacity);
		// copy old data
		std::copy(m_Data_Set.get(), m_Data_Set.get() + m_Size, temp.get());
		// swap ownership to new buffer
		m_Data_Set = std::move(temp);
	}

	void Clear_Set()
	{
		m_Size = 0;
		m_Capacity = 5;
		m_Data_Set = std::make_unique<type[]>(m_Capacity);
	}

	int Set_Cardinality() const { return m_Size; }
	bool Is_Empty() const { return m_Size == 0; }
	int Get_Size() const { return m_Size; }
	int Get_Capacity() const { return m_Capacity; }

	// safe array access
	const type& operator[](int index) const
	{
		if (index < 0 || index >= m_Size)
			throw std::out_of_range("Index out of bounds");
		return m_Data_Set[index];
	}

	// assignment operator
	Math_Set& operator=(const Math_Set& other)
	{
		if (this != &other)
		{
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;
			m_Data_Set = std::make_unique<type[]>(m_Capacity);
			std::copy(other.m_Data_Set.get(), other.m_Data_Set.get() + m_Size, m_Data_Set.get());
		}
		return *this;
	}

	// equality check
	bool operator==(const Math_Set<type>& other) const
	{
		if (m_Size != other.m_Size) return false;
		for (int i = 0; i < m_Size; i++)
			if (m_Data_Set[i] != other.m_Data_Set[i])
				return false;
		return true;
	}

	bool operator!=(const Math_Set<type>& other) const
	{
		return !(*this == other);
	}

	// set comparison is lexicographical to allow sets of sets to be sorted
	bool operator<(const Math_Set<type>& other) const
	{
		if (m_Size != other.m_Size)
			return m_Size < other.m_Size;

		for (int i = 0; i < m_Size; i++)
		{
			if (m_Data_Set[i] != other.m_Data_Set[i])
				return m_Data_Set[i] < other.m_Data_Set[i];
		}
		return false;
	}

	bool operator>(const Math_Set<type>& other) const
	{
		return other < *this;
	}
};

// set operations
// these implementations rely on sorted arrays to perform operations in linear time O(n)

template<Comparable type>
Math_Set<type> UNION(const Math_Set<type>& Set1, const Math_Set<type>& Set2)
{
	Math_Set<type> Set3(Set1.m_Size + Set2.m_Size);
	int j = 0, k = 0;

	// merge sort style logic
	while (j < Set1.m_Size && k < Set2.m_Size)
	{
		if (Set1[j] < Set2[k])
			Set3.Insert_Element(Set1[j++]);
		else if (Set1[j] > Set2[k])
			Set3.Insert_Element(Set2[k++]);
		else
		{
			// if equal insert once and skip both
			Set3.Insert_Element(Set1[j]);
			j++; k++;
		}
	}

	// flush remaining
	while (j < Set1.m_Size) Set3.Insert_Element(Set1[j++]);
	while (k < Set2.m_Size) Set3.Insert_Element(Set2[k++]);

	return Set3;
}

template<Comparable type>
Math_Set<type> INTERSECTION(const Math_Set<type>& Set1, const Math_Set<type>& Set2)
{
	Math_Set<type> Set3(std::min(Set1.m_Size, Set2.m_Size));

	for (int i = 0; i < Set1.m_Size; i++)
	{
		// use binary search to check existence
		int pos = Set2.Binary_Search_Position(Set1[i]);
		if (pos < Set2.m_Size && Set2[pos] == Set1[i])
			Set3.Insert_Element(Set1[i]);
	}
	return Set3;
}

template<Comparable type>
Math_Set<type> DIFFERENCE(const Math_Set<type>& Set1, const Math_Set<type>& Set2)
{
	Math_Set<type> Set3 = Set1;
	for (int i = 0; i < Set2.m_Size; i++)
		Set3.Remove_Element(Set2[i]);
	return Set3;
}

template<Comparable type>
Math_Set<type> SYMMETRIC_DIFFERENCE(const Math_Set<type>& Set1, const Math_Set<type>& Set2)
{
	return UNION(DIFFERENCE(Set1, Set2), DIFFERENCE(Set2, Set1));
}

template<Comparable typeA, Comparable typeB>
Math_Set<std::pair<typeA, typeB>> CARTESIAN_PRODUCT(const Math_Set<typeA>& Set1, const Math_Set<typeB>& Set2)
{
	int capacity1 = Set1.m_Size;
	int capacity2 = Set2.m_Size;
	Math_Set<std::pair<typeA, typeB>> Set3(capacity1 * capacity2);

	// nested loop to pair every element with every other element
	for (int i = 0; i < capacity1; i++)
	{
		for (int j = 0; j < capacity2; j++)
		{
			Set3.Insert_Element({ Set1[i], Set2[j] });
		}
	}
	return Set3;
}

template<Comparable type>
Math_Set<Math_Set<type>> POWER_SET(const Math_Set<type>& Set)
{
	int n = Set.m_Size;
	// bitwise shift to calculate 2^n
	int total = 1 << n;

	Math_Set<Math_Set<type>> result(total);

	// iterate through all numbers from 0 to 2^n - 1
	// treat the binary representation as a mask for which elements to include
	for (int mask = 0; mask < total; mask++)
	{
		Math_Set<type> subset(n);
		for (int i = 0; i < n; i++)
		{
			if (mask & (1 << i))
				subset.Insert_Element(Set[i]);
		}
		result.Insert_Element(subset);
	}
	return result;
}

template<Comparable type>
bool IS_SUBSET(const Math_Set<type>& Set1, const Math_Set<type>& Set2)
{
	for (int i = 0; i < Set1.m_Size; i++)
	{
		int pos = Set2.Binary_Search_Position(Set1[i]);
		if (pos >= Set2.m_Size || Set2[pos] != Set1[i])
			return false;
	}
	return true;
}

template<Comparable type>
bool IS_PROPER_SUBSET(const Math_Set<type>& Set1, const Math_Set<type>& Set2)
{
	// must be a subset AND have fewer elements
	return IS_SUBSET(Set1, Set2) && Set1.m_Size < Set2.m_Size;
}