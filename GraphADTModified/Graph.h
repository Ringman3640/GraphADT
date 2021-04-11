/*
Name: Franz Alarcon
Section: OL
Program Name: Program 4 Graph with Dijkstra's Algorithm

Description: The following is a template Graph class
	which uses Dijkstra's algorithm to find the 
	shortest paths between nodes. The graph uses an
	adjacency list implementation and allows for the 
	addition and removal of vertices and edges. By
	default, each edge has a weight of 1, but can be
	set to any integer amount. The calculations done 
	by Dijkstra's algorithm are saved to a table. 

Issues: I wasn't sure how or why I was supposed to use 
	a incidence matrix. I attempted to implement it at
	first, but adding and removing edges was very
	slow (requied resizing of each vector associated to
	every vertex). I found that it would be much faster
	and simpler for the adjacency list to hold 
	vertex-weight pairs instead of having to manage a
	whole other matrix.
*/

#pragma once
#include <ostream>
#include <unordered_map>
#include <utility>
#include <list>
#include <vector>
#include <limits>

template<typename T>
class Graph {
public:
	Graph() : vertCount{ 0 }, edgeCount{ 0 }, pathTable{ nullptr } {
	
	}

	~Graph() {
		delete pathTable;
	}

	bool addVertex(T vertex) {
		if (vertList.find(vertex) != vertList.end()) return false;
		vertList[vertex] = std::list<std::pair<T, int>>();
		++vertCount;
		return true;
	}

	bool addEdge(T fromVert, T toVert, int weight = 1) {
		if (vertList.find(fromVert) == vertList.end()) return false;
		if (vertList.find(toVert) == vertList.end()) return false;
		vertList[fromVert].push_back(std::make_pair(toVert, weight));
		vertList[toVert].push_back(std::make_pair(fromVert, weight));
		++edgeCount;
		return true;
	}

	bool removeEdge(const T &fromVert, const T &toVert) {
		//Find and remove edge in fromVert
		for (auto it1 = vertList[fromVert].begin(); it1 != vertList[fromVert].end(); ++it1) {
			if (it1->first == toVert) {
				vertList[fromVert].erase(it1);

				//Find and remove edge in toVert
				auto it2 = vertList[toVert].begin();
				for (it2; it2->first != fromVert; ++it2);
				vertList[toVert].erase(it2);
				--edgeCount;
				return true;
			}
		}
		return false;
	}

	bool removeVertex(const T &vertex) {
		if (vertList.find(vertex) == vertList.end()) return false;
		//Erase edges to vertex
		for (auto it1 = vertList[vertex].begin(); it1 != vertList[vertex].end(); ++it1) {
			auto it2 = vertList[it1->first].begin();
			for (it2; it2->first != vertex; ++it2);
			vertList[it1->first].erase(it2);
		}
		//Erase vertex
		vertList.erase(vertex);
		--vertCount;
		return true;
	}

	bool setEdgeWeight(const T &fromVert, const T &toVert, int newWeight = 1) {
		//Find and change weight in fromVert
		for (auto it1 = vertList[fromVert].begin(); it1 != vertList[fromVert].end(); ++it1) {
			if (it1->first == toVert) {
				it1->second = newWeight;

				//Find and change weight in toVert
				auto it2 = vertList[toVert].begin();
				for (it2; it2->first != fromVert; ++it2);
				it2->second = newWeight;
				return true;
			}
		}
		return false;
	}

	void calculatePaths(const T &vertex) {
		if (vertCount == 0 || edgeCount == 0) return;

		//Create new table
		delete pathTable;
		pathTable = new std::unordered_map<T, TableNode>();
		for (auto it : vertList) {
			(*pathTable)[it.first] = TableNode();
		}

		//Initialization
		T currVert = vertex;
		int unvisited = vertCount;
		(*pathTable)[currVert].distance = 0;
		(*pathTable)[currVert].visited = true;
		(*pathTable)[currVert].prev = currVert;

		while (unvisited != 0) {
			//Update adjacent
			if ((*pathTable)[currVert].distance != std::numeric_limits<int>::max())
				for (auto it = vertList[currVert].begin(); it != vertList[currVert].end(); ++it) {
					int tentativeDist = it->second + (*pathTable)[currVert].distance;
					if (tentativeDist < (*pathTable)[it->first].distance) {
						(*pathTable)[it->first].distance = tentativeDist;
						(*pathTable)[it->first].prev = currVert;
					}
				}
			(*pathTable)[currVert].visited = true;
			--unvisited;

			//Get next vertex
			if (unvisited == 0) break;
			auto it = pathTable->begin();
			for (it; it->second.visited; ++it);
			currVert = it->first;
			for (it; it != pathTable->end(); ++it) {
				if (!it->second.visited && it->second.distance < (*pathTable)[currVert].distance) {
					currVert = it->first;
				}
			}
		}
	}

	void printPath(const T &toVert, std::ostream &out) const {
		if (pathTable == nullptr) {
			out << "Paths not calculated" << std::endl;
			return;
		}
		if (pathTable->find(toVert) == pathTable->end()) {
			out << "Vertex " << toVert << " does not exist" << std::endl;
			return;
		}
		if ((*pathTable)[toVert].distance == std::numeric_limits<int>::max()) {
			out << "No path to vertex " << toVert << std::endl;
			return;
		}
		out << toVert << ": ";
		printPathRecursive(toVert, out);
		out << "     Distance: " << (*pathTable)[toVert].distance << std::endl;
	}

	void printAllPaths(std::ostream& out) const {
		if (pathTable == nullptr) {
			out << "Paths not calculated" << std::endl;
			return;
		}
		for (auto it : *pathTable) {
			if (it.second.distance == std::numeric_limits<int>::max()) {
				out << "No path to vertex " << it.first << std::endl;
			}
			else {
				out << it.first << ": ";
				printPathRecursive(it.first, out);
				out << "     Distance: " << it.second.distance << std::endl;
			}
		}
	}

	int getVertCount() const {
		return vertCount;
	}

	int getEdgeCount() const {
		return edgeCount;
	}

private:
	struct TableNode {
		bool visited = false;
		int distance = std::numeric_limits<int>::max();
		T prev;
	};
	std::unordered_map<T, std::list<std::pair<T, int>>> vertList;
	std::unordered_map<T, TableNode> *pathTable;
	int vertCount, edgeCount;

	void printPathRecursive(const T &currNode, std::ostream& out) const {
		if ((*pathTable)[currNode].distance == 0) {
			out << currNode;
			return;
		}
		printPathRecursive((*pathTable)[currNode].prev, out);
		out << " -> " << currNode;
	}
};

//Console output:
/*
STARTING FROM 4
1: 4 -> 3 -> 1     Distance: 4
2: 4 -> 3 -> 2     Distance: 7
3: 4 -> 3     Distance: 2
4: 4     Distance: 0
5: 4 -> 5     Distance: 3
6: 4 -> 5 -> 6     Distance: 4
7: 4 -> 3 -> 8 -> 7     Distance: 6
8: 4 -> 3 -> 8     Distance: 5
9: 4 -> 3 -> 8 -> 9     Distance: 7
*/