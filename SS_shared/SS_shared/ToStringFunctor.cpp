#include "stdafx.h"
#include "ToStringFunctor.h"


ToStringFunctor::ToStringFunctor()
{
}


ToStringFunctor::~ToStringFunctor()
{
}

VarPtr ToStringFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	if (!args.empty() && args[0])
	{
		return VarPtr(new Variable(args[0]->GetStringRepresentation()));
	}
	else return VarPtr(new Variable(""));
}
