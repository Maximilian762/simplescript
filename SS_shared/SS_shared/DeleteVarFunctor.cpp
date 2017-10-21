#include "stdafx.h"
#include "DeleteVarFunctor.h"
#include "Table.h"

DeleteVarFunctor::DeleteVarFunctor()
{
}


DeleteVarFunctor::~DeleteVarFunctor()
{
}

VarPtr DeleteVarFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	VarList varlist;
	for (auto &arg : args)
	{
		if (arg)
		{
			varlist.push_back(arg->GetStringRepresentation());
		}
	}
	int deleted = obj.GetVarContainer().GetRootContainer().DeleteVars(varlist);
	return VarPtr(new Variable(deleted));
}
