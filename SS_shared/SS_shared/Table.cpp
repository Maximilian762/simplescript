#include "stdafx.h"
#include "Table.h"



Table::Table(std::string _name)
{
	name = _name;
}

Table::Table()
{
}

Table::~Table()
{
}

LayeredVarContainer & Table::GetVarContainer()
{
	return vars;
}

MethodContainer & Table::GetMethodContainer()
{
	return methods;
}

std::string Table::GetName()
{
	return name;
}

std::string Table::ToString()
{
	return "table:" + name + "\n" + vars.ToString() + "\n" + methods.ToString();
}

Variable Table::CallMethod(std::string methodname)
{
	return _CallMethodWithParsedArgs(methodname, {});
}

Variable Table::_CallMethodWithParsedArgs(std::string methodname, std::vector<Variable> args)
{
	auto m = methods.GetMethod(methodname);
	if (m)
	{
		std::vector<VarPtr> argsptrs;
		for (auto &v : args) argsptrs.push_back(VarPtr(new Variable(v)));

		auto res = m->Call(*this, argsptrs, IMsgHandler());
		if (res) return *res;
	}

	return Variable();
}
