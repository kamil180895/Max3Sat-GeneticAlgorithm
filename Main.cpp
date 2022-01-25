#include "Problem.h"
#include "Optimizer.h"
#include "Timer.h"

#include <exception>
#include <iostream>

using namespace TimeCounters;

using namespace std;

#define dMAX_TIME 20 * 60


void vRunExperiment(CProblem *pcProblem)
{
	try
	{
		CTimeCounter c_time_counter;

		double d_time_passed;

		COptimizer c_optimizer(pcProblem);

		c_time_counter.vSetStartNow();


		c_optimizer.vInitialize();

		c_time_counter.bGetTimePassed(&d_time_passed);

		while (d_time_passed <= dMAX_TIME)
		{
			c_optimizer.vRunIteration();
			c_optimizer.pcGetBestFound();

			c_time_counter.bGetTimePassed(&d_time_passed);
		}//while (d_time_passed <= MAX_TIME)
		c_optimizer.vAddToReport();
	}//try
	catch (exception &c_exception)
	{
		cout << c_exception.what() << endl;
	}//catch (exception &c_exception)
}//void vRunExperiment(const CEvaluator &cConfiguredEvaluator)

/*
void main(int iArgCount, char **ppcArgValues)
{
	CProblem  c_problem;

	if (c_problem.bLoad("m3s_350_47.txt") == true)
	{
		vRunExperiment(&c_problem);
	}//if (c_problem.bLoad("m3s_100_2.txt") == true)

}//void main(int iArgCount, char **ppcArgValues)
*/

void main(int iArgCount, char **ppcArgValues)
{
	for (int i = 1; i <= 12; ++i)
	{
		CProblem* pc_problem = new CProblem();
		if (pc_problem->bLoad("m3s_350_" + to_string(i) + ".txt") == true)
		{
			vRunExperiment(pc_problem);
		}
		delete pc_problem;
	}
}