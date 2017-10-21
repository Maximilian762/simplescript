#include "stdafx.h"
#include "GetKeysFunctor.h"
#include "Table.h"

GetKeysFunctor::GetKeysFunctor()
{
}


GetKeysFunctor::~GetKeysFunctor()
{
}

VarPtr GetKeysFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	Table *t = new Table();
	t->AddArray(obj.GetVarContainer().GetRootContainer().GetVarNames());
	return VarPtr(new Variable(t));
}
