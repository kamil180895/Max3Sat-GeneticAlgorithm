#include "Problem.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>
#include <sstream>
#include <fstream>

CProblem::CProblem()
{
	pv_variable_tuples = nullptr;
	i_number_of_variables = 0;
}

CProblem::~CProblem()
{
	if (pv_variable_tuples != nullptr)
		delete pv_variable_tuples;
}

bool CProblem::bLoad(std::string sSourcePath)
{
	string outputString;
	ifstream problemFile(sSourcePath);
	

	if (!problemFile.is_open())
		return false;

	if (pv_variable_tuples != nullptr)
	{
		delete pv_variable_tuples;
		i_number_of_variables = 0;
	}
		
	try
	{
		pv_variable_tuples = new vector<vector<CLogicVariable>>();
		while (getline(problemFile, outputString))
		{
			stringstream line(outputString);
			string word;
			int number;

			line >> word;
			pv_variable_tuples->push_back(vector<CLogicVariable>());
			for (int i = 0; i < I_NUMBER_OF_VARIABLES_IN_TUPLE; ++i)
			{
				CLogicVariable logicVariable;
				line >> word;
				if (word[0] == '-')
					logicVariable.vSetNegated(true);
				number = abs(stoi(word));
				logicVariable.vSetPosition(number);
				if (number > i_number_of_variables)
					i_number_of_variables = number;
				pv_variable_tuples->back().push_back(logicVariable);
			}
		}
	}
	catch (std::exception)
	{
		delete pv_variable_tuples;
		pv_variable_tuples = nullptr;
		if (problemFile.is_open())
			problemFile.close();
		return false;
	}
	
	++i_number_of_variables;
	problemFile.close();
	return true;
}

double CProblem::dEvaluate(int * piGenotype)
{
	double result = 0.0;
	for (int i = 0; i < pv_variable_tuples->size(); ++i)
	{
		bool first = (*pv_variable_tuples)[i][0].bIsNegated() ? !piGenotype[(*pv_variable_tuples)[i][0].iGetPosition()] : piGenotype[(*pv_variable_tuples)[i][0].iGetPosition()];
		bool second = (*pv_variable_tuples)[i][1].bIsNegated() ? !piGenotype[(*pv_variable_tuples)[i][1].iGetPosition()] : piGenotype[(*pv_variable_tuples)[i][1].iGetPosition()];
		bool third = (*pv_variable_tuples)[i][2].bIsNegated() ? !piGenotype[(*pv_variable_tuples)[i][2].iGetPosition()] : piGenotype[(*pv_variable_tuples)[i][2].iGetPosition()];
		if (first || second || third)
			result += 1.0;
	}

	return result / pv_variable_tuples->size();
}

int CProblem::iGetNumberVariables()
{
	return i_number_of_variables;
}
