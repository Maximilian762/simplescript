#include "stdafx.h"
#include "AST_Assignment.h"
#include "AST_Variable.h"
#include "Table.h"
#include "AST_MemberAccess.h"
#include "SSInterpreter.h"
using namespace std;

AST_Assignment::AST_Assignment(SSInterpreter &_interp) : interp(_interp)
{
	SetType(AST_Type::Assignment);
}


AST_Assignment::~AST_Assignment()
{
}


void AST_Assignment::SetVar(SharedExpr var)
{
	dst = var;
}

void AST_Assignment::SetVarValue(SharedExpr val)
{
	value = val;
}

void AST_Assignment::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "assignment:" << endl;

	if (dst) dst->PrintSelf(level + 1);
	else
	{
		PrintAlignment(level + 1);
		cout << "<dst empty>" << endl;
	}

	if (value) value->PrintSelf(level + 1);
	else
	{
		PrintAlignment(level + 1);
		cout << "<value empty>" << endl;
	}
}

bool AST_Assignment::AddNewVarsToVarContainer(Table &obj, bool temp, IMsgHandler &errhandler)
{
	VarPtr value_res;

	if (value)
	{
		if (!value->Evaluate(obj, value_res, errhandler) || !value_res)
		{
			errhandler.OnMsg("Couldnt evaluate assignment value");
			return false;
		}
	}
	else //uninitialised var
	{
		Variable *emptyvar = new Variable();
		auto newvar = dynamic_pointer_cast<AST_Variable>(dst);

		if (newvar) emptyvar->SetType(newvar->GetType());
		if (emptyvar->Type() == VarType::Table /*&& newvar->GetType() != "table"*/)
		{
			shared_ptr<Table> customclass_table(interp.InstantiateTable(newvar->GetType()));
			emptyvar->BecomeTable(customclass_table);
		}
		value_res = VarPtr(emptyvar);
	}

	if (auto newvar = dynamic_pointer_cast<AST_Variable>(dst))
	{
		if (temp)
		{
			if (obj.GetVarContainer().HasVar(newvar->GetName()))
			{
				VarPtr existingvar = obj.GetVarContainer().GetVarPtr(newvar->GetName());
				if (existingvar)
				{
					*existingvar = *value_res;
					return true;
				}
				else
				{
					errhandler.OnMsg("Couldnt retrieve existing variable ptr: " + newvar->GetName());
					return false;
				}
			}
			else
			{
				obj.GetVarContainer().GetTopContainer().AddVar(newvar->GetName(), value_res);
				return true;
			}
		}
		else
		{
			obj.GetVarContainer().GetRootContainer().AddVar(newvar->GetName(), value_res);
			return true;
		}
	}
	else if (auto newvar = dynamic_pointer_cast<AST_MemberAccess>(dst))
	{
		VarPtr dstvar;
		newvar->SetCreatingOnRequestStatus(true);
		bool success = newvar->Evaluate(obj, dstvar, errhandler);
		newvar->SetCreatingOnRequestStatus(false);

		if (dstvar && success)
		{
			*dstvar = *value_res;
			return true;
		}
	}

	errhandler.OnMsg("Assignment couldnt evaluate destination");
	return false;
}

bool AST_Assignment::Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler)
{
	bool ret = AddNewVarsToVarContainer(obj, true, errhandler);
	if (!ret)
	{
		errhandler.OnMsg("Assignment evaluation failed");
	}
	return ret;
}
