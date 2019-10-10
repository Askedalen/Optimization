#pragma once
class StartingSolutionGenerator
{
public:
	StartingSolutionGenerator(std::vector<std::vector<int>>* t);
	std::vector<int> createRandomSolution();
	std::vector<int> createIterativeRandomSolution();
	std::vector<int> createGreedySolution();
	int calcCost(std::vector<int>* solution);
	~StartingSolutionGenerator();
private:
	std::vector<std::vector<int>>* costTable;
};

