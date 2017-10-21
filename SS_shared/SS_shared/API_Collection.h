#pragma once
#include "AST_Function.h"
#include "AST_Class.h"
#include <map>
#include "TableTemplate.h"

class API_Collection
{
public:
	API_Collection();
	~API_Collection();

	void AddMethod(std::string classname, std::string methodname, SharedFunctor f, SSInterpreter &interpreter);
	void HandleClassDefinitionTemplate(TableTemplate &temp);
	void AppendOtherApi(API_Collection & other);

	std::string InfoToString();

private:
	std::map<std::string, std::vector<SharedFunction>> methods;
};

