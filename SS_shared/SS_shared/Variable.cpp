#include "stdafx.h"
#include "Variable.h"
#include "Table.h"
using namespace std;

Variable::Variable()
{
}

Variable::Variable(int _c)
{
	type = VarType::Int;
	type_name = "int";
	intcontent = _c;
}

Variable::Variable(float _c)
{
	type = VarType::Float;
	type_name = "float";
	floatcontent = _c;
}

Variable::Variable(std::string _c)
{
	type = VarType::String;
	type_name = "string";
	stringcontent = _c;
}

Variable::Variable(std::shared_ptr<Table> _c)
{
	type = VarType::Table;
	type_name = _c->GetName();
	tablecontent = _c;
}

Variable::Variable(Table * _c)
{
	type = VarType::Table;
	type_name = _c->GetName();
	tablecontent = std::shared_ptr<Table>(_c);
}

Variable::Variable(const Variable & other, bool independent)
{
	*this = Variable(other);
	if (other.tablecontent) tablecontent = std::shared_ptr<Table>(new Table(*other.tablecontent));
}

Variable::~Variable()
{
}

VarType Variable::Type()
{
	return type;
}

bool Variable::IntOrFloat()
{
	return type == VarType::Int || type == VarType::Float;
}

float Variable::GetNumber()
{
	return (type == VarType::Int ? intcontent : floatcontent);
}

int & Variable::GetIntRef()
{
	return intcontent;
}

float & Variable::GetFloatRef()
{
	return floatcontent;
}

std::string & Variable::GetStringRef()
{
	return stringcontent;
}

std::shared_ptr<Table> Variable::GetTablePtr()
{
	return tablecontent;
}

bool Variable::SameType(Variable & other)
{
	return type == other.type;
}

std::string Variable::GetTypeName()
{
	return type_name;
}

std::string Variable::ToString()
{
	string content;
	if (type == VarType::Unassigned) content = "nil";
	else if (type == VarType::Int) content = "int:" + to_string(intcontent);
	else if (type == VarType::Float) content = "float:" + to_string(floatcontent);
	else if (type == VarType::String) content = "string:" + stringcontent;
	else if (type == VarType::Table)
	{
		if (tablecontent) content = tablecontent->ToString();
		else content = "niltable";
	}

	return content;
}

std::string Variable::GetStringRepresentation()
{
	if (type == VarType::Unassigned) return "";
	else if (type == VarType::Int) return to_string(intcontent);
	else if (type == VarType::Float) return to_string(floatcontent);
	else if (type == VarType::String) return stringcontent;
	else if (type == VarType::Table) return "<table>";
}

void Variable::SetType(std::string type_name)
{
	if (type_name == "int") type = VarType::Int;
	else if (type_name == "float") type = VarType::Float;
	else if (type_name == "string") type = VarType::String;
	else if (type_name == "") type = VarType::Unassigned;
	else type = VarType::Table;
}

void Variable::BecomeTable(std::shared_ptr<Table> _c)
{
	type = VarType::Table;
	type_name = _c->GetName();
	tablecontent = _c;
}
