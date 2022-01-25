#pragma once

#include <string>
#include <vector>
#include "LogicVariable.h"

#define I_NUMBER_OF_VARIABLES_IN_TUPLE 3

using namespace std;

class CProblem
{
public:
	CProblem();
	
	virtual ~CProblem();

	bool  bLoad(std::string sSourcePath);

	double dEvaluate(int *piGenotype);

	int iGetNumberVariables();
protected:
	vector<vector<CLogicVariable>>* pv_variable_tuples;
	int i_number_of_variables;
};//class CProblem


