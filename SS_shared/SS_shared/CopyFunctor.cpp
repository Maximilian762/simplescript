#include "stdafx.h"
#include "CopyFunctor.h"


CopyFunctor::CopyFunctor()
{
}


CopyFunctor::~CopyFunctor()
{
}

VarPtr CopyFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	if (!args.empty() && args[0])
	{
		return VarPtr(new Variable(*args[0], true));
	}
	else return VarPtr(new Variable(0));
}
