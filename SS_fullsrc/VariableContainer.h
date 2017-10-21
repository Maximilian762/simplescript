#pragma once
#include <map>
#include "Variable.h"

class VariableContainer
{
public:
	VariableContainer();
	VariableContainer(const VariableContainer& other);
	~VariableContainer();

	void AddVar(std::string name, Variable v);
	void AddVar(std::string name, VarPtr v);

	bool HasVar(std::string name);
	VarPtr GetVarPtr(std::string name);
	int DeleteVars(VarList list);

	std::string ToString();

	int GetLastNumerableVarName();
	int GetVarsCount();
	std::vector<std::string> GetVarNames();

private:
	std::map<std::string, VarPtr> vars;
};

