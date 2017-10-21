#include "stdafx.h"
#include "GetSizeFunctor.h"
#include "Table.h"

GetSizeFunctor::GetSizeFunctor()
{
}


GetSizeFunctor::~GetSizeFunctor()
{
}

VarPtr GetSizeFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	return VarPtr(new Variable(obj.GetVarContainer().GetRootContainer().GetVarsCount()));
}
