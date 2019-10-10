#include "pch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>

#include "StartingSolutionGenerator.h"

StartingSolutionGenerator::StartingSolutionGenerator(std::vector<std::vector<int>>* t)
{
	costTable = t;
	srand(time(NULL));
}

std::vector<int> StartingSolutionGenerator::createRandomSolution()
{
	std::cout << "Creating random solution..." << std::endl;
	std::vector<int> solution;
	for (int i = 0; i < costTable->size(); i++) {
		int citynr;
		do {
			citynr = rand() % costTable->size();
		} while (std::find(solution.begin(), solution.end(), citynr) != solution.end());
		solution.push_back(citynr);
	}

	return solution;
}

std::vector<int> StartingSolutionGenerator::createIterativeRandomSolution()
{
	std::cout << "Creating iterative random solution..." << std::endl;
	int solutionCount = 10;
	std::vector<std::vector<int>> solutions(solutionCount);
	for (int i = 0; i < solutionCount; i++) {
		std::vector<int> currentSolution;
		for (int j = 0; j < costTable->size(); j++) {
			int citynr;
			do {
				citynr = rand() % costTable->size();
			} while (std::find(currentSolution.begin(), currentSolution.end(), citynr) != currentSolution.end());
			currentSolution.push_back(citynr);
		}
		solutions[i] = currentSolution;
	}
	int lowestCost = 300000;
	std::vector<int>* bestSolution(0);
	for (int i = 0; i < solutions.size(); i++) {
		std::vector<int>* vec = &solutions[i];
		int cost = calcCost(vec);
		if (cost < lowestCost) {
			lowestCost = cost;
			bestSolution = vec;
		}
	}
	return *bestSolution;
}

std::vector<int> StartingSolutionGenerator::createGreedySolution()
{
	/*
	This method uses greedy method to generate a starting solution
	*/
	std::cout << "Creating greedy solution..." << std::endl;
	std::vector<int> solution; // Creates a new solution vector with the same size as costTable
	// Chooses a random starting city and puts the starting city into the solution start
	int citynr = rand() % costTable->size(); 
	solution.push_back(citynr); 
	for (int i = 1; i < costTable->size(); i++) { // For number of cities
		// Gets the column from costTable, with distances to all the other cities from the current last city in solution
		std::vector<int>* vec = &costTable->at(solution[i-1]); 
		// Initiates variables for the lowest cost and the index with the lowest cost
		int lowestCost = 100; 
		int lowestCostj = -1;
		for (int j = 0; j < vec->size(); j++) { // For every element in vec
			int cost = (*vec)[j]; // Finds cost at j in vec
			//if cost is less than lowestCost AND cost is not 0 AND j does not already exist in solution:
			if (cost < lowestCost
			 && std::find(solution.begin(), solution.end(), j) == solution.end()) {
				// New lowest cost and lowest cost index
				lowestCost = cost;
				lowestCostj = j;
			}
		}
		// Push lowest cost city to solution.
		solution.push_back(lowestCostj);
	}
	return solution;
}

int StartingSolutionGenerator::calcCost(std::vector<int>* solution)
{
	int cost = 0;
	for (int i = 1; i < solution->size(); i++) {
		cost += (*costTable)[(*solution)[i]][(*solution)[i - 1]];
	}
	cost += (*costTable)[(*solution)[0]][(*solution).back()];
	return cost;
}

StartingSolutionGenerator::~StartingSolutionGenerator()
{
}
