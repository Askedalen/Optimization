#pragma once
class Optimizer
{
public:
	Optimizer(std::vector<std::vector<int>>* t, std::vector<int>* s);
	std::vector<int> greedyOptimize();
	std::vector<int> greedyRandomOptimize();
	int calcCost(std::vector<int>* solution);
	~Optimizer();
private:
	std::vector<std::vector<int>>* costTable;
	std::vector<int>* initialSolution;
};

