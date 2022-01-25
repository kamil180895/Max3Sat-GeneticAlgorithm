#include "LogicVariable.h"

CLogicVariable::CLogicVariable()
{
	b_is_negated = false;
	i_position = 0;
}

CLogicVariable::CLogicVariable(bool bIsNegated, int iPosition)
{
	b_is_negated = bIsNegated;
	i_position = iPosition;
}

bool CLogicVariable::bIsNegated()
{
	return b_is_negated;
}

int CLogicVariable::iGetPosition()
{
	return i_position;
}

void CLogicVariable::vSetNegated(bool bIsNegated)
{
	b_is_negated = bIsNegated;
}

void CLogicVariable::vSetPosition(int iPosition)
{
	i_position = iPosition;
}
