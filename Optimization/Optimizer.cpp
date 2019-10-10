#include "pch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <chrono>

#include "Optimizer.h"

Optimizer::Optimizer(std::vector<std::vector<int>>* t, std::vector<int>* s)
{
	costTable = t;
	initialSolution = s;
}

std::vector<int> Optimizer::greedyOptimize()
{
	std::cout << "Greedy optimize..." << std::endl;
	srand(time(NULL));
	std::vector<int> solution = *initialSolution;
	std::vector<int> temporarySolution = solution;
	std::ofstream file("greedyOptimizeAnalytic.txt");
	file.clear();

	int prevTimeInSeconds = 0;

	int city1Nr;
	int city1;
	int city2Nr;
	int city2;
	int newCost;
	int bestCost = calcCost(initialSolution);
	signed long long startTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	for (int i = 0; i < 50000; i++) {
		city1Nr = rand() % costTable->size();
		city1 = solution[city1Nr];
		do {
			city2Nr = rand() % costTable->size();
			city2 = solution[city2Nr];
		} while (city2 == city1);
		temporarySolution[city1Nr] = city2;
		temporarySolution[city2Nr] = city1;
		newCost = calcCost(&temporarySolution);
		if (newCost <= bestCost) {
			bestCost = newCost;
			solution = temporarySolution;
		} else {
			temporarySolution = solution;
		}
		signed long long timestampMcrs = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		signed long timeSinceStartMicrs = timestampMcrs - startTime;
		file << timeSinceStartMicrs << " " << bestCost << std::endl;
		int timeInSeconds = (int)(timeSinceStartMicrs / 1000000);
		if (timeInSeconds != prevTimeInSeconds) {
			std::cout << "Time: " << timeInSeconds << "\r";
			prevTimeInSeconds = timeInSeconds;
		}
	}
	std::cout << std::endl;
	file.close();
	return solution;
}

std::vector<int> Optimizer::greedyRandomOptimize()
{
	std::cout << "Greedy random optimize..." << std::endl;
	srand(time(NULL));
	std::vector<int> solution = *initialSolution;
	std::vector<int> temporarySolution = solution;
	std::vector<int> bestSolution = solution;
	std::ofstream file("greedyRandomOptimizeAnalytic.txt");
	file.clear();

	// Used if POA should be raised after it reaches a lower limit.
	//int poaChanged = 0;

	int prevTimeInSeconds = 0;
	double poa = 0.9;
	int city1Nr;
	int city1;
	int city2Nr;
	int city2;
	int newCost;
	int initialCost = calcCost(initialSolution);
	int oldCost = initialCost;
	int bestCost = initialCost;
	signed long long startTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	int j = 0;
	do {
		for (int i = 0; i < 500; i++) {
			city1Nr = rand() % costTable->size();
			city1 = solution[city1Nr];
			do {
				city2Nr = rand() % costTable->size();
				city2 = solution[city2Nr];
			} while (city2 == city1);
			temporarySolution[city1Nr] = city2;
			temporarySolution[city2Nr] = city1;
			newCost = calcCost(&temporarySolution);
			if (newCost <= oldCost) {
				oldCost = newCost;
				solution = temporarySolution;
				if (newCost <= bestCost) {
					bestCost = newCost;
					bestSolution = temporarySolution;
				}
			} else {
				double r = rand() % 10000;
				double rnd = r / 10000;
				if (rnd < poa) {
					oldCost = newCost;
					solution = temporarySolution;
				} else {
					temporarySolution = solution;
				}
			}
			signed long long timestampMcrs = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			signed long timeSinceStartMicrs = timestampMcrs - startTime;
			file << timeSinceStartMicrs << " " << oldCost << std::endl;
			int timeInSeconds = (int)(timeSinceStartMicrs / 1000000);
			if (timeInSeconds != prevTimeInSeconds) {
				std::cout << "Time: " << timeInSeconds << "\r";
				prevTimeInSeconds = timeInSeconds;
			}
			j++;
		}
		poa = poa * 0.9;

		// Uncomment this to raise POA once it is lowered to 0.00001
		//if (poa < 0.00001 && poaChanged <= 5) {
		//	poa = 0.1;
		//	poaChanged++;
		//}

	} while (poa > 0.00001);
	file.close();
	std::cout << std::endl;
	std::cout << "j: " << j << std::endl;
	return bestSolution;
}

int Optimizer::calcCost(std::vector<int>* solution)
{
	int cost = 0;
	for (int i = 1; i < solution->size(); i++) {
		cost += (*costTable)[(*solution)[i]][(*solution)[i - 1]];
	}
	cost += (*costTable)[(*solution)[0]][(*solution).back()];
	return cost;
}

Optimizer::~Optimizer()
{
}
