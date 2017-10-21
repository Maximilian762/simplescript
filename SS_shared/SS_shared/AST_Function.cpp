#include "stdafx.h"
#include "AST_Function.h"
#include "Table.h"

using namespace std;

AST_Function::AST_Function(SSInterpreter &_interpreter, std::string _name, bool _returns_ref) : interp(_interpreter)
{
	SetType(AST_Type::Function);
	name = _name;
	returns_ref = _returns_ref;
}

AST_Function::AST_Function(SSInterpreter & _interpreter, std::string _name, SharedFunctor _functor) : interp(_interpreter)
{
	SetType(AST_Type::Function);
	name = _name;
	functor = _functor;
}


AST_Function::~AST_Function()
{
}

std::vector<SharedExpr>& AST_Function::GetCode()
{
	return code;
}

void AST_Function::SetCode(std::vector<SharedExpr> thecode)
{
	code = thecode;
}

void AST_Function::SetArguments(std::vector<SharedVar> theargs)
{
	args = theargs;
}

void AST_Function::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "function:" << endl;

	if (functor)
	{
		PrintAlignment(level);
		cout << "CPP FUNCTOR" << endl;
		return;
	}

	PrintAlignment(level+1);
	cout << "args:" << endl;

	for (auto &var : args)
	{
		if (var) var->PrintSelf(level + 2);
		else
		{
			PrintAlignment(level + 2);
			cout << "<emptyarg>" << endl;
		}
	}

	PrintAlignment(level+1);
	cout << "code:" << endl;

	for (auto &expr : code) expr->PrintSelf(level + 2);
}

std::string AST_Function::GetName()
{
	return name;
}

VarPtr AST_Function::Call(Table& obj, std::vector<VarPtr> callargs, IMsgHandler &errhandler)
{
	if (functor) return functor->Call(interp, obj, callargs, errhandler);

	if (args.size() == callargs.size())
	{
		/*for (int i = 0; i < args.size(); ++i)
		{
			if (args[i] && callargs[i] && !args[i]->SameType(*callargs[i]))
			{
				errhandler.OnMsg("Function call: args type dont match");
				return VarPtr();
			}
		}*/
	}
	else
	{
		errhandler.OnMsg("Expected different number of args in func call");
		return VarPtr();
	}

	obj.GetVarContainer().PushNewContainer();

	for (int i = 0; i < args.size(); ++i)
	{
		string varname = args[i]->GetName();

		VarPtr argvalue; 
		if (args[i]->IsReference())
		{
			argvalue = callargs[i];
		}
		else
		{
			argvalue = VarPtr(new Variable(*callargs[i], true));
		}

		obj.GetVarContainer().GetTopContainer().AddVar(varname, argvalue);
	}

	VarPtr res;
	for (auto &expr : code)
	{
		if (!expr)
		{
			errhandler.OnMsg("Nullptr expr in function code");

			obj.GetVarContainer().PopContainer();
			return ReturnValue(res);
		}

		bool success = expr->Evaluate(obj, res, errhandler);
		if (expr->GetType() == AST_Type::Return || !success)
		{
			break;
		}
	}

	obj.GetVarContainer().PopContainer();
	return ReturnValue(res);
}

std::string AST_Function::ToString()
{
	return "function:" + name;
}

VarPtr AST_Function::ReturnValue(VarPtr rawresult)
{
	if (returns_ref || !rawresult) return rawresult;
	else return VarPtr(new Variable(*rawresult, true));
}
