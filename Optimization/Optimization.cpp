// Optimization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include "StartingSolutionGenerator.h"
#include "Optimizer.h"

std::vector<std::vector<int>> costTable;
std::vector<int> initialSolution;
std::vector<int> optimizedSolution;

void printVector() {
	std::cout << "      ";
	for (int x = 0; x < costTable.size(); x++) {
		std::cout << (x < 9 ? "0" : "") << x + 1 << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < costTable.size(); i++) {
		std::cout << (i < 9 ? "0" : "") << i + 1 << " || ";
		for (int j = 0; j < costTable.size(); j++) {
			int cellContent = costTable[i][j];
			std::cout << (cellContent < 10 ? "0" : "") << cellContent << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void fillCostTable() {
	std::string filename;
	std::cout << "Filename: ";
	std::getline(std::cin, filename);

	std::ifstream file(filename);
	std::string line;
	int i = 0;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			costTable.resize(i + 1);
			costTable[i] = std::vector<int>();
			std::stringstream stream(line);
			int num;
			while (stream >> num) {
				costTable[i].push_back(num);
			}
			i++;
		}
	} else {
		std::cout << "Cannot open file!" << std::endl;
	}
	file.close();
	if (costTable.size() <= 50) printVector();
}

void printSolution(std::vector<int> solution) {
	std::cout << "Solution: " << std::endl;
	for (int citynr : solution) {
		std::cout << citynr+1 << " ";
	}
	std::cout << std::endl;
}

int calculateCost(std::vector<int>* solution) {
	int cost = 0;
	for (int i = 1; i < solution->size(); i++) {
		cost += costTable[(*solution)[i]][(*solution)[i - 1]];
	}
	cost += costTable[(*solution)[0]][(*solution).back()];
	return cost;
}

int main()
{
	fillCostTable();
	do {
		StartingSolutionGenerator ssg(&costTable);
		int method;
		std::cout << "Choose method for generating starting solution." << std::endl;
		std::cout << "1 - random, 2 - iterative random, 3 - greedy" << std::endl;
		std::cin >> method;
		switch (method)
		{
		case 1:
			initialSolution = ssg.createRandomSolution();
			break;
		case 2:
			initialSolution = ssg.createIterativeRandomSolution();
			break;
		case 3:
			initialSolution = ssg.createGreedySolution();
			break;
		default:
			initialSolution = ssg.createRandomSolution();
			break;
		}
		printSolution(initialSolution);
		std::cout << "Cost: " << calculateCost(&initialSolution) << std::endl;
		std::cout << "Press Enter to optimize..." << std::endl;
		std::cin.ignore();
		do {
			Optimizer opt(&costTable, &initialSolution);
			int optmethod;
			std::cout << "Choose method for optimizing." << std::endl;
			std::cout << "1 - greedy, 2 - greedy random (POA)" << std::endl;
			std::cin >> optmethod;
			switch (optmethod)
			{
			case 1:
				optimizedSolution = opt.greedyOptimize();
				break;
			case 2:
				optimizedSolution = opt.greedyRandomOptimize();
				break;
			default:
				optimizedSolution = opt.greedyRandomOptimize();
				break;
			}
			if (costTable.size() <= 50) printSolution(optimizedSolution);
			std::cout << "Cost " << calculateCost(&optimizedSolution) << std::endl;
			std::cout << "Difference: " << (calculateCost(&initialSolution) - calculateCost(&optimizedSolution)) << std::endl;

			char a;
			std::cout << "Optimize again? (y/n)" << std::endl;
			std::cin >> a;
			std::cout << std::endl;
			if (a != 'y') {
				break;
			}
		} while (true);
		char a;
		std::cout << "Create new initial solution? (y/n)" << std::endl;
		std::cin >> a;
		std::cout << std::endl;
		if (a != 'y') {
			break;
		}
	} while (true);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
