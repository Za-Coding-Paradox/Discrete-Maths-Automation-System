#pragma once

#include "Critical_Node_Generator.h"

// We use a dual representation for the graph:
// Adjacency Matrix (using map-of-maps), Provides O(1) lookup to check if an edge exists or get its weight.
// Adjacency List, Provides efficient O(degree) iteration over a node's neighbors, which is critical for traversal algorithms like DFS/BFS.
template <Allowed_Node_Type type>
class Graph
{
private:
	bool m_Is_Directed;

	// We use unordered_map<type, unordered_map<type, int>> instead of a 2D array [N][N].
	// Why?
	// Sparse Graphs, Real-world graphs often have few edges compared to nodes. A 2D array wastes huge memory (N^2) storing zeros.
	// Flexible Node Types, Array indices must be integers 0..N. Our nodes can be strings ("A", "B") or non-sequential ints (10, 500). Maps handle this naturally.
	std::unordered_map<type, std::unordered_map<type, int>> m_Adjacency_Matrix;

	// Stores a list of direct neighbors for each node.
	// loops in pathfinding.
	std::unordered_map<type, std::list<std::pair<type, int>>> m_Adjacency_List;

public:
	Graph(bool is_Directed = false)
	{
		m_Is_Directed = is_Directed;
	}

	// This function pre-registers all nodes into the graph structure.
	// Why is this needed?
	// If a graph has a node "C" that is isolated (no edges connect to it), it wouldn't appear in Add_Edge calls.
	// By iterating the input set first, we ensure "C" exists in our maps with an empty neighbor list.
	void Initialize_Graph(const Math_Set<type>& nodes)
	{
		int count = nodes.Get_Size();
		for (int i = 0; i < count; i++)
		{
			const type& A = nodes[i];
			// The [] operator for maps has a special behavior:
			// If the key 'A' doesn't exist, it creates it with a default value (empty list/map) automatically.
			m_Adjacency_List[A];
			m_Adjacency_Matrix[A];
		}
	}

	// Adds a weighted connection between two nodes.
	void Add_Edge(type node_A, type node_B, int weight)
	{
		// Ensure nodes exist first (in case Initialize_Graph wasn't called or new nodes are introduced)
		m_Adjacency_List[node_A];
		m_Adjacency_Matrix[node_A];
		m_Adjacency_List[node_B];
		m_Adjacency_Matrix[node_B];

		// For Node A, add B to its list and matrix
		m_Adjacency_List[node_A].push_back({ node_B, weight });
		m_Adjacency_Matrix[node_A][node_B] = weight;

		// If the graph is undirected (A-B is same as B-A), we must mirror the edge.
		if (!m_Is_Directed)
		{
			m_Adjacency_List[node_B].push_back({ node_A, weight });
			m_Adjacency_Matrix[node_B][node_A] = weight;
		}
	}

	// Removes a connection.
	void Remove_Edge(type node_A, type node_B)
	{
		// Basic check, can't remove edge from a non-existent node
		if (m_Adjacency_List.find(node_A) == m_Adjacency_List.end()) return;

		// std::list::remove_if takes a "predicate" (a function that returns true/false).
		// It takes 'node_B' from the outer scope and checks if the current item matches it.
		m_Adjacency_List[node_A].remove_if(
			[node_B](const std::pair<type, int>& node)
			{
				return node.first == node_B;
			});

		// Removing from Matrix is simpler: just set weight to 0 or remove key.
		// Setting to 0 effectively removes the weight.
		m_Adjacency_Matrix[node_A][node_B] = 0;

		// Mirror for undirected graphs
		if (!m_Is_Directed)
		{
			if (m_Adjacency_List.find(node_B) == m_Adjacency_List.end()) return;

			m_Adjacency_List[node_B].remove_if(
				[node_A](const std::pair<type, int>& node)
				{
					return node.first == node_A;
				});

			m_Adjacency_Matrix[node_B][node_A] = 0;
		}
	}

	bool Has_Edge(type node_A, type node_B)
	{
		// First check if A exists in the outer map
		if (m_Adjacency_Matrix.find(node_A) == m_Adjacency_Matrix.end())
			return false;

		// Then check if B exists in A's inner map
		if (m_Adjacency_Matrix[node_A].find(node_B) == m_Adjacency_Matrix[node_A].end())
			return false;

		// Finally, check if the weight is non-zero (assuming 0 means no edge)
		return static_cast<bool>(m_Adjacency_Matrix[node_A][node_B]);
	}

	int Get_Weight(type node_A, type node_B)
	{
		return m_Adjacency_Matrix[node_A][node_B];
	}

	// A circuit visits every edge exactly once and returns to start.
	// Rules vary by graph type:
	// Undirected, Graph is connected AND every node has an Even degree.
	// Directed, Graph is strongly connected AND In-Degree == Out-Degree for every node.
	bool Is_Eulerian_Circuit() const
	{
		if (Get_Num_Vertices() == 0) return false;

		if (m_Is_Directed)
		{
			// Track in/out degrees for all nodes
			std::unordered_map<type, int> in_degree, out_degree;

			for (const auto& [A, row] : m_Adjacency_Matrix)
			{
				for (const auto& [B, w] : row)
				{
					if (w != 0)
					{
						out_degree[A]++; // A has an edge leaving
						in_degree[B]++;  // B has an edge entering
					}
				}
			}

			// If graph is split into sub graphs, you can't traverse all edges.
			if (Count_Connected_Components() > 1) return false;

			// Degree check
			for (const auto& [vertex, row] : m_Adjacency_Matrix)
			{
				if (in_degree[vertex] != out_degree[vertex])
					return false; // Flow in must equal flow out
			}
			return true;
		}
		else
		{
			// Undirected Case
			if (Count_Connected_Components() > 1) return false;

			for (const auto& [A, Edges] : m_Adjacency_Matrix)
			{
				int degree = 0;
				for (const auto& [B, weight] : Edges)
				{
					if (weight != 0) degree++;
				}
				// If any node has an odd number of edges, we get stuck at that node
				if (degree % 2 != 0) return false;
			}
			return true;
		}
	}

	// Similar to circuit, but start and end can be different.
	// Condition, Exactly 0 or 2 nodes have an odd degree.
	bool Is_Eulerian_Path() const
	{
		if (Get_Num_Vertices() == 0) return false;

		int odd_count = 0;
		for (const auto& [A, Edges] : m_Adjacency_Matrix)
		{
			int degree = 0;
			for (const auto& [B, Weight] : Edges)
				if (Weight != 0) degree++;

			if (degree % 2 != 0) odd_count++;
		}
		return (odd_count == 0 || odd_count == 2);
	}

	// A cycle that visits every NODE exactly once.
	// This is an NP-Complete problem, meaning there is no known fast algorithm.
	// Using Depth First Search (DFS) with backtracking to try every possible path.
	bool Has_Hamiltonian_Cycle() const
	{
		if (Get_Num_Vertices() == 0) return false;

		std::unordered_map<type, bool> visited;
		std::vector<type> hamiltonian_path;

		// Initialize all as unvisited
		for (const auto& [node, _] : m_Adjacency_List)
		{
			visited[node] = false;
		}

		// Pick arbitrary start node (cycles include all nodes, so start doesn't matter)
		const type& start = visited.begin()->first;

		visited[start] = true;
		hamiltonian_path.push_back(start);

		// Launch recursive search
		return Hamiltonian_DFS(visited, hamiltonian_path, start);
	}

	// Recursive Backtracking Helper
	bool Hamiltonian_DFS(std::unordered_map<type, bool>& visited_nodes,
		std::vector<type>& hamiltonian_path,
		const type& current_node) const
	{
		// Base Case: Have we visited every node?
		if (hamiltonian_path.size() == Get_Num_Vertices())
		{
			// We have a full path. Now check if the last node connects back to start.
			type startNode = hamiltonian_path.front();

			bool connectedToStart = false;
			for (const auto& edge : m_Adjacency_List.at(current_node)) {
				if (edge.first == startNode && edge.second != 0) {
					connectedToStart = true;
					break;
				}
			}
			return connectedToStart;
		}

		// Recursively, Try all valid neighbors
		const auto& neighbours = m_Adjacency_List.at(current_node);
		for (const auto& [dfs_node, weight] : neighbours)
		{
			// If neighbor is unvisited
			if (weight != 0 && !visited_nodes[dfs_node])
			{
				// Add to path
				visited_nodes[dfs_node] = true;
				hamiltonian_path.push_back(dfs_node);

				// Explore
				if (Hamiltonian_DFS(visited_nodes, hamiltonian_path, dfs_node))
					return true; // Found, return true/backtrack

				// Backtrack nodes to Remove from path to try different neighbor next loop
				visited_nodes[dfs_node] = false;
				hamiltonian_path.pop_back();
			}
		}

		// If no neighbors lead to a solution
		return false;
	}

	// Checks if every node is connected to every other node.
	// For N nodes, each node must have N-1 edges.
	bool Is_Complete() const
	{
		size_t numV = Get_Num_Vertices();
		for (const auto& [A, list] : m_Adjacency_List)
		{
			if (list.size() < numV - 1) return false;
		}
		return true;
	}

	// Counts how many disjoint "sub graphs" exist in the graph.
	// Uses standard Iterative DFS.
	int Count_Connected_Components() const
	{
		if (Get_Num_Vertices() == 0) return 0;

		std::unordered_map<type, bool> visited;
		for (const auto& [node, _] : m_Adjacency_List) visited[node] = false;

		int component_count = 0;
		std::stack<type> dfs_stack;

		// Iterate over every node. If it hasn't been visited yet, it's the start of a new component.
		for (const auto& [start_node, _] : m_Adjacency_List)
		{
			if (visited[start_node]) continue;

			// Found a new unvisited island
			component_count++;
			dfs_stack.push(start_node);
			visited[start_node] = true;

			// Flood fill (DFS) to mark all nodes in this island
			while (!dfs_stack.empty())
			{
				type current = dfs_stack.top();
				dfs_stack.pop();

				for (const auto& [neighbor, weight] : m_Adjacency_List.at(current))
				{
					if (weight != 0 && !visited[neighbor])
					{
						visited[neighbor] = true;
						dfs_stack.push(neighbor);
					}
				}
			}
		}
		return component_count;
	}

	const std::unordered_map<type, std::list<std::pair<type, int>>>& Get_Adjacency_List() const
	{
		return m_Adjacency_List;
	}

	int Get_Edge_Weight_From_Matrix(type node_A, type node_B) const
	{
		if (m_Adjacency_Matrix.count(node_A) && m_Adjacency_Matrix.at(node_A).count(node_B))
			return m_Adjacency_Matrix.at(node_A).at(node_B);
		return 0;
	}

	int Get_Num_Vertices() const
	{
		return (int)m_Adjacency_List.size();
	}

	int Get_Num_Edges() const
	{
		int count = 0;
		for (const auto& [node, neighbors] : m_Adjacency_List)
		{
			count += neighbors.size();
		}

		// If undirected, each edge is stored twice (A->B and B->A), so divide by 2
		if (!m_Is_Directed)
		{
			return count / 2;
		}
		return count;
	}

	int Get_Degree(const type& node) const
	{
		if (m_Adjacency_List.find(node) != m_Adjacency_List.end())
		{
			return (int)m_Adjacency_List.at(node).size();
		}
		return 0;
	}

	bool Is_Directed() const
	{
		return m_Is_Directed;
	}

	std::vector<type> Get_All_Nodes() const
	{
		std::vector<type> nodes;
		for (const auto& [node, _] : m_Adjacency_List)
		{
			nodes.push_back(node);
		}
		return nodes;
	}

	// Output stream operator
	friend std::ostream& operator<<(std::ostream& os, const Graph<type>& g)
	{
		os << "\n" << (g.m_Is_Directed ? "Directed" : "Undirected") << " Graph (" << g.Get_Num_Vertices() << " nodes):\n";
		for (const auto& [node, neighbors] : g.m_Adjacency_List)
		{
			std::stringstream ss;
			ss << "[" << node << "]";
			// Formatting with setw creates nice aligned columns
			os << "  " << std::left << std::setw(10) << ss.str() << " -> ";
			if (neighbors.empty()) {
				os << "None";
			}
			else {
				bool first = true;
				for (const auto& [neighbor, weight] : neighbors)
				{
					if (!first) os << ", ";
					os << neighbor;
					if (weight != 1) os << "(w:" << weight << ")";
					first = false;
				}
			}
			os << "\n";
		}
		return os;
	}
};