// GraphProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Graph.h"

int main()
{
	Graph<int> graph;

	for (int i = 1; i <= 9; i++)
	{
		graph.addVertex(i);
	}

	//1
	graph.addEdge(1, 2, 4);
	graph.addEdge(1, 3, 2);

	//2
	graph.addEdge(2, 3, 5);
	graph.addEdge(2, 9, 7);

	//3
	graph.addEdge(3, 4, 2);
	graph.addEdge(3, 6, 9);
	graph.addEdge(3, 8, 3);

	//4
	graph.addEdge(4, 5, 3);

	//5
	graph.addEdge(5, 6, 1);
	graph.addEdge(5, 7, 8);

	//6
	graph.addEdge(6, 7, 5);
	
	//7
	graph.addEdge(7, 8, 1);

	//8
	graph.addEdge(8, 9, 2);

	//9
	//All edges addes

	//Path tests
	std::cout << "STARTING FROM 4" << std::endl;
	graph.calculatePaths(4);
	graph.printAllPaths(std::cout);
}