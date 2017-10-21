#include "stdafx.h"
#include "AST_Variable.h"
#include <iostream>
#include "Variable.h"
#include "Table.h"

using namespace std;

AST_Variable::AST_Variable(std::string _type, std::string _name, std::string _value)
{
	SetType(AST_Type::Variable);
	type = _type;
	name = _name;
	value = _value;
}

AST_Variable::~AST_Variable()
{
}

void AST_Variable::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "var: " << type << " " + name << " = " << (value.empty() ? "<empty>" : value);
	if (is_reference) cout << " <REFERENCE>";
	cout << endl;
}

void AST_Variable::BecomeReference()
{
	is_reference = true;
}

bool AST_Variable::IsReference()
{
	return is_reference;
}

std::string AST_Variable::GetType()
{
	return type;
}

std::string AST_Variable::GetName()
{
	return name;
}

std::string AST_Variable::GetValue()
{
	return value;
}

bool AST_Variable::SameType(Variable & arg)
{
	return arg.GetTypeName() == type;
}

bool AST_Variable::Evaluate(Table & obj, VarPtr &return_var, IMsgHandler &errhandler)
{
	return_var = obj.GetVarContainer().GetVarPtr(name);
	return true;
}
