#include "Optimizer.h"

#include <cfloat>
#include <iostream>
#include <windows.h>
#include <fstream>

#undef max

using namespace std;

COptimizer::COptimizer(CProblem * pcProblem)
	:c_rand_engine(time(nullptr))
{
	pc_problem = pcProblem;
	d_crossover_prob = DEF_CROSSOVER_PROB;
	d_mutation_prob = DEF_MUTATION_PROB;
	i_population_size = DEF_POPULATION_SIZE;
	s_report_file = DEF_REPORT_FILE_NAME;
	pc_best_found = new CIndividual(this);
}

COptimizer::~COptimizer()
{
	for (CIndividual* itr : v_population)
	{
		delete itr;
	}
	delete pc_best_found;
}

void COptimizer::vInitialize()
{
	for (CIndividual* itr : v_population)
	{
		delete itr;
	}

	v_population.resize(i_population_size);
	for (int i = 0; i < i_population_size; ++i)
	{
		v_population[i] = new CIndividual(this);
	}
}

void COptimizer::vRunIteration()
{
	vector<CIndividual*> newPopulation;
	CIndividual* parent1 = nullptr;
	CIndividual* parent2 = nullptr;
	vector<CIndividual*>* children = nullptr;
	while (newPopulation.size() < v_population.size())
	{
		parent1 = pcChooseParent();
		parent2 = pcChooseParent();
		children = parent1->pcCrossover(parent2);
		(*children)[0]->vMutation();
		(*children)[1]->vMutation();
		newPopulation.push_back((*children)[0]);
		newPopulation.push_back((*children)[1]);
		if ((*children)[0]->dFitness() > pc_best_found->dFitness())
		{
			delete pc_best_found;
			pc_best_found = new CIndividual(*(*children)[0]);
		}
		if ((*children)[1]->dFitness() > pc_best_found->dFitness())
		{
			delete pc_best_found;
			pc_best_found = new CIndividual(*(*children)[1]);
		}
		delete children;
	}

	
	for (CIndividual* itr : v_population)
	{
		delete itr;
	}

	v_population = newPopulation;
}

CIndividual * COptimizer::pcGetBestFound()
{
	return pc_best_found;
}

CIndividual * COptimizer::pcChooseParent()
{
	int randomIndividual1 = c_rand_engine() % i_population_size;
	int randomIndividual2 = c_rand_engine() % i_population_size;

	if (v_population[randomIndividual1]->dFitness() > v_population[randomIndividual2]->dFitness())
		return v_population[randomIndividual1];
	else
		return v_population[randomIndividual2];
}

double COptimizer::dRandomFraction()
{
	return ((double)c_rand_engine()) / c_rand_engine.max();
}

void COptimizer::vAddToReport()
{
	ofstream reportFile(s_report_file, ios::out | ios::app);

	if (!reportFile.is_open())
		return;

	reportFile << pc_best_found->dFitness() << " ";
	for (int j = 0; j < pc_problem->iGetNumberVariables(); ++j)
	{
		reportFile << pc_best_found->v_genotype[j];
	}
	reportFile << " ";

	for (int i = 0; i < i_population_size; ++i)
	{
		reportFile << v_population[i]->dFitness();
		reportFile << " ";
	}
	reportFile << "\n";

	reportFile.close();
}

CIndividual::CIndividual(COptimizer* cOptimizer)
{
	i_genotype_size = cOptimizer->pc_problem->iGetNumberVariables();
	v_genotype = new int[i_genotype_size];
	pc_optimizer = cOptimizer;
	d_fitness_score = DEF_FITNESS_SCORE;
	vRandomizeGenotype();
}

CIndividual::CIndividual(const CIndividual & pcOther)
{
	v_genotype = new int[pcOther.i_genotype_size];
	i_genotype_size = pcOther.i_genotype_size;
	for (int i = 0; i < i_genotype_size; ++i)
	{
		v_genotype[i] = pcOther.v_genotype[i];
	}
	d_fitness_score = pcOther.d_fitness_score;
	pc_optimizer = pcOther.pc_optimizer;
}

CIndividual::~CIndividual()
{
	delete[] v_genotype;
}

vector<CIndividual*>* CIndividual::pcCrossover(const CIndividual * pcParent2)
{
	CIndividual* child1 = new CIndividual(*this);
	CIndividual* child2 = new CIndividual(*pcParent2);
	vector<CIndividual*>* children = new vector<CIndividual*>();
	children->push_back(child1);
	children->push_back(child2);

	if (pc_optimizer->dRandomFraction() > pc_optimizer->d_crossover_prob)
		return children;

	child1->d_fitness_score = -1.0;
	child2->d_fitness_score = -1.0;

	for (int i = 0; i < i_genotype_size; ++i)
	{
		if (pc_optimizer->c_rand_engine() % 2 == 0)
		{
			child1->v_genotype[i] = v_genotype[i];
			child2->v_genotype[i] = pcParent2->v_genotype[i];
		}
		else
		{
			child1->v_genotype[i] = pcParent2->v_genotype[i];
			child2->v_genotype[i] = v_genotype[i];
		}
	}

	return children;
}

void CIndividual::vMutation()
{
	if (pc_optimizer->dRandomFraction() > pc_optimizer->d_mutation_prob)
		return;

	int randomGene = pc_optimizer->c_rand_engine() % i_genotype_size;

	v_genotype[randomGene] = !v_genotype[randomGene];

	d_fitness_score = -1.0;
}

double CIndividual::dFitness()
{
	if (d_fitness_score >= 0.0)
		return d_fitness_score;
	
	d_fitness_score = pc_optimizer->pc_problem->dEvaluate(v_genotype);

	return d_fitness_score;
}

void CIndividual::vRandomizeGenotype()
{
	for (int i=0; i < i_genotype_size; ++i)
	{
		v_genotype[i] = pc_optimizer->c_rand_engine() % 2;
	}
}

int * CIndividual::piGetResultAsTable()
{
	int* result = new int[i_genotype_size];
	for (int i = 0; i < i_genotype_size; ++i)
	{
		result[i] = v_genotype[i];
	}

	return result;
}
