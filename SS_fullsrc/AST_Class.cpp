#include "stdafx.h"
#include "AST_Class.h"
#include <iostream>
using namespace std;


AST_Class::AST_Class(std::string _classname)
{
	SetType(AST_Type::Class);
	classname = _classname;
}

AST_Class::~AST_Class()
{
}

void AST_Class::AddVariableAssignments(std::vector<SharedExpr>& assignments, IMsgHandler &errhandler)
{
	for (auto &expr : assignments)
	{
		if (auto assignment = dynamic_pointer_cast<AST_Assignment>(expr))
		{
			variable_assignments.push_back(assignment);
		}
		else
		{
			errhandler.OnMsg(classname + " class error at adding variable assignments: expression is not an assignment or is empty");
		}
	}
}

std::vector<SharedAssignment>& AST_Class::GetVariableAssignments()
{
	return variable_assignments;
}

std::vector<SharedFunction>& AST_Class::GetFunctions()
{
	return functions;
}

void AST_Class::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "<class>" << endl;
	for (auto &v : variable_assignments) v->PrintSelf(level + 1);
	for (auto &f : functions) f->PrintSelf(level + 1);
}

TableTemplate AST_Class::CreateTableTemplate()
{
	TableTemplate t;
	t.SetName(classname);
	t.SetBaseClasses(baseclassnames);

	for (auto &v : variable_assignments) t.AddVarAssignment(v);
	for (auto &f : functions) t.GetMethodContainer().AddMethod(f->GetName(), f);

	return t;
}

void AST_Class::AddBaseClass(std::string classname)
{
	baseclassnames.push_back(classname);
}

std::string AST_Class::GetClassName()
{
	return classname;
}
