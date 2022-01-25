#pragma once

#include "Problem.h"

#include <random>
#include <vector>
#include <time.h>

#define DEF_CROSSOVER_PROB 0.3
#define DEF_MUTATION_PROB 0.3
#define DEF_POPULATION_SIZE 30
#define DEF_REPORT_FILE_NAME "Report.txt"

#define DEF_FITNESS_SCORE -1.0
using namespace std;

class  CIndividual;

class COptimizer
{
	friend class CIndividual;
public:
	COptimizer(CProblem *pcProblem);
	~COptimizer();

	void vInitialize();
	void vRunIteration();
	void vAddToReport();

	CIndividual  *pcGetBestFound();
private:
	CIndividual* pcChooseParent();
	double dRandomFraction();// <0,1>
	
	CProblem *pc_problem;
	mt19937 c_rand_engine;
	CIndividual  *pc_best_found;

	vector<CIndividual *> v_population;

	double d_crossover_prob;
	double d_mutation_prob;
	int i_population_size;
	string s_report_file;
};//class COptimizer


class  CIndividual
{
	friend class COptimizer;
public:
	CIndividual(COptimizer* pcOptimizer);
	CIndividual(const CIndividual& pcOther);
	~CIndividual();

	vector<CIndividual*>* pcCrossover(const CIndividual* pcParent2);
	void vMutation();
	double dFitness();

	void vRandomizeGenotype();

	int* piGetResultAsTable();
private:
	int* v_genotype;
	int i_genotype_size;
	COptimizer* pc_optimizer;

	double d_fitness_score;
};//class  CIndividual