#pragma once

class CLogicVariable
{
public:
	CLogicVariable();
	CLogicVariable(bool bIsNegated, int iPosition);

	bool bIsNegated();
	int iGetPosition();

	void vSetNegated(bool bIsNegated);
	void vSetPosition(int iPosition);
private:
	bool b_is_negated;
	int i_position;
};