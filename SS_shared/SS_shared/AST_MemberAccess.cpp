#include "stdafx.h"
#include "AST_MemberAccess.h"
#include "Variable.h"
#include "Table.h"
using namespace std;

AST_MemberAccess::AST_MemberAccess(std::string _objname, SharedExpr _member)
{
	SetType(AST_Type::MemberAccess);
	objname = _objname;
	memberexpr = _member;
}

AST_MemberAccess::~AST_MemberAccess()
{
}

void AST_MemberAccess::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "access: " + objname << endl;

	if (memberexpr) memberexpr->PrintSelf(level + 1);
	else
	{
		PrintAlignment(level+1);
		cout << "<emptymemberaccessexpr>" << endl;
	}
}

bool AST_MemberAccess::Evaluate(Table & obj, VarPtr &return_var, IMsgHandler &errhandler)
{
	if (!memberexpr)
	{
		errhandler.OnMsg("Empty member access expr");
		return false;
	}

	VarPtr member;
	bool success = memberexpr->Evaluate(obj, member, errhandler);

	if (!success)
	{
		errhandler.OnMsg("Couldnt evaluate member access expr inside braces");
	}

	Table *caller = &obj;
	if (objname != "this")
	{
		if (auto callerptr = obj.GetVarContainer().GetVarPtr(objname)->GetTablePtr())
		{
			caller = callerptr.get();
		}
		else
		{
			errhandler.OnMsg("Couldnt retrieve member access obj " + objname);
			return false;
		}
	}

	string actualvarname = member->GetStringRepresentation();

	if (create_at_request && !caller->GetVarContainer().HasVar(actualvarname))
	{
		caller->GetVarContainer().GetRootContainer().AddVar(actualvarname, Variable());
	}

	return_var = caller->GetVarContainer().GetVarPtr(actualvarname);

	return true;
}

void AST_MemberAccess::SetCreatingOnRequestStatus(bool _create_at_request)
{
	create_at_request = _create_at_request;
}
