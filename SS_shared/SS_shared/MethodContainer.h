#pragma once
#include "AST_Function.h"
#include <map>
#include <string>
#include <memory>

class MethodContainer
{
public:
	MethodContainer();
	~MethodContainer();

	bool HasMethod(std::string name);
	void AddMethod(std::string name, SharedFunction meth);
	SharedFunction GetMethod(std::string name);

	std::string ToString();

	void Append(MethodContainer &other);

private:
	std::map<std::string, SharedFunction> methods;
};

