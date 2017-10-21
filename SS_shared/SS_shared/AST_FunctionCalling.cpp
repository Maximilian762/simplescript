#include "stdafx.h"
#include "AST_FunctionCalling.h"
#include "Table.h"

using namespace std;

AST_FunctionCalling::AST_FunctionCalling(std::string _callername, std::string _name, std::vector<SharedExpr>& _arguments)
{
	SetType(AST_Type::FunctionCalling);
	callername = _callername;
	name = _name;
	arguments = _arguments;
}

AST_FunctionCalling::~AST_FunctionCalling()
{
}

std::string AST_FunctionCalling::GetName()
{
	return name;
}

std::vector<SharedExpr>& AST_FunctionCalling::GetArgs()
{
	return arguments;
}

void AST_FunctionCalling::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "call: " << callername << "->" << name << endl;
	PrintAlignment(level + 1);

	if (arguments.empty()) cout << "noargs" << endl;
	else
	{
		cout << "args:" << endl;

		for (auto &expr : arguments)
		{
			if (expr) expr->PrintSelf(level + 2);
			else
			{
				PrintAlignment(level + 2);
				cout << "<emptyarg>" << endl;
			}
		}
	}
}

bool AST_FunctionCalling::Evaluate(Table & obj, VarPtr &return_var, IMsgHandler &errhandler)
{
	Table *caller = &obj;
	if (callername != "this")
	{
		bool exists = obj.GetVarContainer().GetVarPtr(callername).operator bool();
		if (exists)
		{
			if (auto callerptr = obj.GetVarContainer().GetVarPtr(callername)->GetTablePtr())
			{
				caller = callerptr.get();
			}
			else
			{
				errhandler.OnMsg("Couldnt retrieve tableptr from caller");
				return false;
			}
		}
		else
		{
			errhandler.OnMsg("Couldnt retrieve function caller: " + callername);
			return false;
		}
	}

	if (auto method = caller->GetMethodContainer().GetMethod(name))
	{
		VarPtr res = method->Call(*caller, EvaluateArguments(obj, errhandler), errhandler);
		return_var = res;
		return true;
	}
	else
	{
		errhandler.OnMsg("Couldnt retrieve method " + name + " from " + callername);
		return false;
	}
}

std::vector<VarPtr> AST_FunctionCalling::EvaluateArguments(Table & obj, IMsgHandler &errhandler)
{
	std::vector<VarPtr> evaluatedargs;
	for (auto &expr : arguments)
	{
		VarPtr ret;
		bool success = expr->Evaluate(obj, ret, errhandler);
		evaluatedargs.push_back(ret);
	}
	return evaluatedargs;
}
