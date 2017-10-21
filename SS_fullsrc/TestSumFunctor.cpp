#include "stdafx.h"
#include "TestSumFunctor.h"


TestSumFunctor::TestSumFunctor()
{
}


TestSumFunctor::~TestSumFunctor()
{
}

VarPtr TestSumFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	int result = 0;

	for (auto &arg : args)
	{
		if (!arg) continue;

		result += arg->GetIntRef();
	}

	return VarPtr(new Variable(result));
}
