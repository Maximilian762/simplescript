#include "stdafx.h"
#include "VariableContainer.h"


VariableContainer::VariableContainer()
{
}

VariableContainer::VariableContainer(const VariableContainer & other)
{
	for (auto it = other.vars.begin(); it != other.vars.end(); it++)
	{
		vars[it->first] = VarPtr(new Variable(*it->second, true));
	}
}

VariableContainer::~VariableContainer()
{
}

void VariableContainer::AddVar(std::string name, Variable v)
{
	vars[name] = VarPtr(new Variable(v));
}

void VariableContainer::AddVar(std::string name, VarPtr v)
{
	vars[name] = v;
}

bool VariableContainer::HasVar(std::string name)
{
	return vars.find(name) != vars.end();
}

VarPtr VariableContainer::GetVarPtr(std::string name)
{
	auto found = vars.find(name);
	if (found != vars.end())
	{
		return found->second;
	}
	else return nullptr;
}

int VariableContainer::DeleteVars(VarList list)
{
	int deleted = 0;

	/*auto it = vars.begin();
	while (it != vars.end())
	{
		for (auto &vname : list)
		{
			if (it->first == vname)
			{
				it = vars.erase(it);
				deleted++;
				break;
			}
		}
		++it;
	}*/

	for (auto &vname : list)
	{
		auto found = vars.find(vname);
		if (found != vars.end())
		{
			vars.erase(found);
			deleted++;
		}
	}

	return deleted;
}

std::string VariableContainer::ToString()
{
	std::string info = "variables:";
	for (auto it = vars.begin(); it != vars.end(); it++)
	{
		info = info + "\n" + it->first + " " + it->second->ToString();
	}
	return info;
}

int VariableContainer::GetLastNumerableVarName()
{
	if (!vars.empty())
	{
		std::string highest = vars.rbegin()->first;
		bool isnum = true;
		for (int i = 0; i < highest.size(); ++i) { if (!isdigit(highest[i])) { isnum = false; break; } }

		if (isnum) return stoi(highest);
	}

	return -1;
}

int VariableContainer::GetVarsCount()
{
	return vars.size();
}

std::vector<std::string> VariableContainer::GetVarNames()
{
	std::vector<std::string> names;
	for (auto it = vars.begin(); it != vars.end(); ++it)
	{
		names.push_back(it->first);
	}
	return names;
}
