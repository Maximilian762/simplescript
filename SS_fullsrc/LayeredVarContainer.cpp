#include "stdafx.h"
#include "LayeredVarContainer.h"

using namespace std;

LayeredVarContainer::LayeredVarContainer()
{
	PushNewContainer();
}

LayeredVarContainer::~LayeredVarContainer()
{
}

void LayeredVarContainer::PushNewContainer()
{
	containers.push_back(VariableContainer());
}

void LayeredVarContainer::PopContainer()
{
	containers.pop_back();
}

VariableContainer & LayeredVarContainer::GetTopContainer()
{
	return containers.back();
}

VariableContainer & LayeredVarContainer::GetRootContainer()
{
	return containers.front();
}

bool LayeredVarContainer::HasVar(std::string name)
{
	for (int i = containers.size() - 1; i >= 0; --i)
	{
		if (containers[i].HasVar(name))
		{
			return true;
		}
	}
	return false;
}

VarPtr LayeredVarContainer::GetVarPtr(std::string name)
{
	for (int i = containers.size() - 1; i >= 0; --i)
	{
		if (auto varptr = containers[i].GetVarPtr(name))
		{
			return varptr;
		}
	}
	return nullptr;
}

std::string LayeredVarContainer::ToString()
{
	string allinfo;
	for (auto &vc : containers) allinfo += vc.ToString() + "\n";
	return allinfo;
}
