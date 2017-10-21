#include "stdafx.h"
#include "HasVarFunctor.h"
#include "Table.h"

HasVarFunctor::HasVarFunctor()
{
}


HasVarFunctor::~HasVarFunctor()
{
}

VarPtr HasVarFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	int varspresent_count = 0;

	for (auto &arg : args)
	{
		if (!arg) continue;

		if (obj.GetVarContainer().GetRootContainer().HasVar(arg->GetStringRepresentation()))
		{
			varspresent_count++;
		}
	}

	return VarPtr(new Variable(varspresent_count));
}
