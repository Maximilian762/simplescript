#pragma once
#include "IFunctor.h"
class TestSumFunctor :
	public IFunctor
{
public:
	TestSumFunctor();
	virtual ~TestSumFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

