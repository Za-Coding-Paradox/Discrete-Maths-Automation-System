#pragma once

#include "Graph.h"

// Struct to package the results of a pathfinding query.
// It keeps data plain (public members) because it's just a data transfer object.
template <Allowed_Node_Type type>
struct Path_Result
{
public:
	// The sequence of nodes from Start to End.
	std::vector<type> m_Path;
	// Sum of edge weights. Default is infinite.
	int m_Total_Cost = INT_MAX;
	// Quick success/fail check.
	bool m_Path_Exists = false;

public:
	Path_Result() = default;

	// Formats the path like [A->B->C] for display.
	friend std::ostream& operator<<(std::ostream& out, const Path_Result<type>& result)
	{
		out << "\n";
		out << "PATH: ";

		if (!result.m_Path_Exists || result.m_Path.empty())
		{
			out << "[NONE]\n";
			out << "COST: INF" << std::endl;
			out << "STATUS: FAILED" << std::endl;
			return out;
		}

		out << "[";
		for (size_t i = 0; i < result.m_Path.size(); i++)
		{
			out << result.m_Path[i];
			if (i + 1 < result.m_Path.size())
				out << "->";
		}
		out << "]\n";
		out << "COST: " << result.m_Total_Cost << std::endl;
		out << "STATUS: SUCCESS" << std::endl;

		return out;
	}
};

// Finds the path with the minimum total weight between two nodes.
//
// How it works (Greedy Approach):
// Maintain a "Minimum Distance" to every node, initialized to Infinity. Start node is 0.
// Use a Priority Queue (Min-Heap) to always process the "closest known node" next.
// For the current node, check all neighbors.
// Relaxation, If (Dist to Current + Edge Weight) < (Known Dist to Neighbor), we found a better path! Update the neighbor's distance and parent.
template <Allowed_Node_Type type>
class Dijikstras_Shortest_Path
{
public:
	Dijikstras_Shortest_Path() = delete;

	static Path_Result<type> Dijikstras_Shortest_Path_Calculation(
		const Graph<type>& graph,
		const type& start_node,
		const type& end_node)
	{
		Path_Result<type> shortest_path;

		const auto& list = graph.Get_Adjacency_List();

		// f start or end nodes aren't in the graph, return immediately.
		if (!list.count(start_node) || !list.count(end_node))
			return shortest_path;

		// Data structures for the algorithm
		std::unordered_map<type, bool> visited_node;      // To avoid processing nodes twice (though Dijkstra allows revisit if shorter, simple bool helps optimization)
		std::unordered_map<type, int> minimum_distance;   // The best distance found so far to each node
		std::unordered_map<type, type> parent_node;       // treconstruct the path backwards

		auto all_nodes = graph.Get_All_Nodes();

		// Initialization Step
		for (const auto& node : all_nodes)
		{
			visited_node[node] = false;
			minimum_distance[node] = INT_MAX; // Assume infinite distance initially
		}

		minimum_distance[start_node] = 0;

		// Priority Queue:
		// Stores {distance, node}.
		// 'std::greater' makes it a Min-Heap (smallest distance pops first).
		// This is the core engine of Dijkstra, ensuring we expand the "frontier" efficiently.
		std::priority_queue <
			std::pair<int, type>,
			std::vector<std::pair<int, type>>,
			std::greater<>
		> priority_queue;

		priority_queue.push({ 0, start_node });

		while (!priority_queue.empty())
		{
			// Get the closest node
			auto [distance, current_node] = priority_queue.top();
			priority_queue.pop();

			// If we already found a shorter way to this node previously, skip this stale entry.
			if (visited_node[current_node])
				continue;

			visited_node[current_node] = true;

			// If we reached the target, we can stop.
			if (current_node == end_node)
				break;

			// Iterate Neighbors
			for (const auto& [neighbor, weight] : list.at(current_node))
			{
				if (weight == 0) continue; // Zero weight often implies no edge in adjacency matrix representation

				int new_distance = minimum_distance[current_node] + weight;

				// If the path through 'current_node' shorter than the previous best known path to 'neighbor'
				if (new_distance < minimum_distance[neighbor])
				{
					minimum_distance[neighbor] = new_distance;
					parent_node[neighbor] = current_node; // Record where we came from
					priority_queue.push({ new_distance, neighbor }); // Add to queue for exploration
				}
			}
		}

		// Check if destination is still unreachable
		if (minimum_distance[end_node] == INT_MAX)
			return shortest_path;

		// We trace backwards from End -> Parent -> Parent ... -> Start
		type child = end_node;
		while (parent_node.count(child))
		{
			shortest_path.m_Path.push_back(child);
			child = parent_node.at(child);
		}
		shortest_path.m_Path.push_back(start_node);

		// The path is currently End->Start, so reverse it.
		std::reverse(shortest_path.m_Path.begin(), shortest_path.m_Path.end());

		shortest_path.m_Path_Exists = true;
		shortest_path.m_Total_Cost = minimum_distance[end_node];

		return shortest_path;
	}
};

// Result structure for Minimum Spanning Tree
template <Allowed_Node_Type type>
struct MST_Result
{
	// Stores the actual edges of the tree: pair<pair<from, to>, weight>
	std::vector<std::pair<std::pair<type, type>, int>> m_Edges;
	int m_Total_Weight;
	bool m_MST_Exists;

public:
	MST_Result() : m_Total_Weight(0), m_MST_Exists(false)
	{
	}

	friend std::ostream& operator<<(std::ostream& os, const MST_Result<type>& result)
	{
		os << "\nMST EDGES:" << std::endl;
		for (const auto& edge : result.m_Edges)
		{
			const auto& [from, to] = edge.first;
			int weight = edge.second;

			os << "(" << from << "," << to << "): " << weight << std::endl;
		}

		os << "TOTAL WEIGHT: " << result.m_Total_Weight << std::endl;
		os << "STATUS: " << (result.m_MST_Exists ? "SUCCESS" : "FAILED") << std::endl;

		return os;
	}
};

// Finds a subset of edges that connects all vertices with the minimum possible total weight.
//
// How it works:
// Grow a tree from a starting node.
// At each step, add the cheapest edge that connects a node Inside the tree to a node Outside the tree.
// Repeat until all nodes are included.
//
// Constraint Feature:
// This implementation adds a check to ensure specific "Must Include" nodes are part of the final connected component.
template <Allowed_Node_Type type>
class Constrained_Prims_MST
{
public:
	Constrained_Prims_MST() = delete;

	static MST_Result<type> Constrained_Prims_MST_Calculation(
		const Graph<type>& graph,
		const Math_Set<type>& must_include_nodes,
		const type& starting_node)
	{
		MST_Result<type> mst_result;

		const int num_vertices = graph.Get_Num_Vertices();

		if (num_vertices == 0)
			return mst_result;

		// MST fundamentally requires the graph to be connected.
		// If there are multiple disconnected islands, we can't span them all.
		if (graph.Count_Connected_Components() > 1)
			return mst_result;

		std::vector<type> all_nodes = graph.Get_All_Nodes();

		// Prim's Data Structures
		std::unordered_map<type, bool> in_mst;        // Is node included in the tree yet?
		std::unordered_map<type, int> node_keys;      // Minimum weight edge connecting this node to the current tree
		std::unordered_map<type, std::optional<type>> parent_nodes; // The neighbor that offers that minimum weight

		// Initialize
		for (const auto& node : all_nodes)
		{
			in_mst[node] = false;
			node_keys[node] = INT_MAX;
			parent_nodes[node] = std::nullopt; // Using std::optional handles the root node having no parent
		}

		// Start with the first node (cost to reach itself is 0)
		node_keys[starting_node] = 0;

		// Loop N times to add N vertices
		for (int count = 0; count < num_vertices; count++)
		{
			// Pick the node with the smallest key that isn't in MST yet.
			// (A priority queue could optimize this O(N) search to O(log N)), I realized this after implementation.
			type min_node;
			int min_key = INT_MAX;
			bool found = false;

			for (const auto& node : all_nodes)
			{
				if (!in_mst[node] && node_keys[node] < min_key)
				{
					min_key = node_keys[node];
					min_node = node;
					found = true;
				}
			}

			if (!found)
				break; // Should not happen if graph is connected

			// Add node to MST
			in_mst[min_node] = true;

			// Update key values of adjacent vertices
			// If a neighbor is not in MST, and the edge from 'min_node' is cheaper than its current key, update it.
			for (const auto& neighbor : all_nodes)
			{
				int weight = graph.Get_Edge_Weight_From_Matrix(min_node, neighbor);

				if (weight != 0 && !in_mst[neighbor] && weight < node_keys[neighbor])
				{
					node_keys[neighbor] = weight;
					parent_nodes[neighbor] = min_node;
				}
			}
		}

		// Ensure every "must include" node actually made it into the tree.
		for (int i = 0; i < must_include_nodes.Get_Size(); i++)
		{
			if (!in_mst[must_include_nodes[i]])
			{
				return mst_result; // Failed constraints
			}
		}

		// Construct the result object from the parent pointers
		for (const auto& node : all_nodes)
		{
			if (parent_nodes[node].has_value())
			{
				type parent_node = parent_nodes[node].value();
				int weight = graph.Get_Edge_Weight_From_Matrix(parent_node, node);

				std::pair<type, type> edge_pair = { parent_node, node };
				std::pair<std::pair<type, type>, int> edge_with_weight = { edge_pair, weight };

				mst_result.m_Edges.push_back(edge_with_weight);
				mst_result.m_Total_Weight += weight;
			}
		}

		// An MST for N nodes must have exactly N-1 edges.
		mst_result.m_MST_Exists = (mst_result.m_Edges.size() == num_vertices - 1);

		return mst_result;
	}
};