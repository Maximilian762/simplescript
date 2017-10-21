#pragma once
#include "AST_Assignment.h"
#include <vector>
#include "MethodContainer.h"

class TableTemplate
{
public:
	TableTemplate();
	~TableTemplate();

	void AddVarAssignment(SharedAssignment ass);
	MethodContainer& GetMethodContainer();

	void InstantiateTable(Table *t, std::map<std::string, TableTemplate> &alltemplates);

	void SetName(std::string _name);
	std::string GetName();

	void SetBaseClasses(std::vector<std::string> &bases);

private:
	std::string name;

	std::vector<std::string> baseclasses;

	std::vector<SharedAssignment> varassignments;
	MethodContainer methods;
};

