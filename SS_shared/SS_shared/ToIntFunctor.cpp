#include "stdafx.h"
#include "ToIntFunctor.h"
#include "common.h"

ToIntFunctor::ToIntFunctor()
{
}


ToIntFunctor::~ToIntFunctor()
{
}

VarPtr ToIntFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	int res = 0;
	if (!args.empty() && args[0])
	{
		try {
			res = stoi(args[0]->GetStringRepresentation());
		}
		catch (...)
		{
			return VarPtr(new Variable(0));
		}

		return VarPtr(new Variable(res));
	}
	else return VarPtr(new Variable(0));
}
