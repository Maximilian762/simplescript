#pragma once
#include "VariableContainer.h"
#include <vector>

class LayeredVarContainer
{
public:
	LayeredVarContainer();
	~LayeredVarContainer();

	void PushNewContainer();
	void PopContainer();

	VariableContainer& GetTopContainer();
	VariableContainer& GetRootContainer();

	bool HasVar(std::string name);
	VarPtr GetVarPtr(std::string name);

	std::string ToString();

private:
	std::vector<VariableContainer> containers;
};

