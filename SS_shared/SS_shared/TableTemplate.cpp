#include "stdafx.h"
#include "TableTemplate.h"
#include "Table.h"
using namespace std;

TableTemplate::TableTemplate()
{
}


TableTemplate::~TableTemplate()
{
}

void TableTemplate::AddVarAssignment(SharedAssignment ass)
{
	varassignments.push_back(ass);
}

MethodContainer & TableTemplate::GetMethodContainer()
{
	return methods;
}

void TableTemplate::InstantiateTable(Table *t, std::map<std::string, TableTemplate> &alltemplates)
{
	for (string basename : baseclasses)
	{
		auto foundtemplate = alltemplates.find(basename);
		if (foundtemplate != alltemplates.end())
		{
			foundtemplate->second.InstantiateTable(t, alltemplates);
		}
		else
		{
			//base class template not found...
			return;
		}
	}

	t->GetMethodContainer().Append(methods);
	for (auto &ass : varassignments) ass->AddNewVarsToVarContainer(*t, false, IMsgHandler());
}

void TableTemplate::SetName(std::string _name)
{
	name = _name;
}

std::string TableTemplate::GetName()
{
	return name;
}

void TableTemplate::SetBaseClasses(std::vector<std::string>& bases)
{
	baseclasses = bases;
}
