#include "stdafx.h"
#include "ToFloatFunctor.h"


ToFloatFunctor::ToFloatFunctor()
{
}


ToFloatFunctor::~ToFloatFunctor()
{
}

VarPtr ToFloatFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	float res = 0;
	if (!args.empty() && args[0])
	{
		try {
			res = stof(args[0]->GetStringRepresentation());
		}
		catch (...)
		{
			return VarPtr(new Variable(0.0f));
		}

		return VarPtr(new Variable(res));
	}
	else return VarPtr(new Variable(0.0f));
}
