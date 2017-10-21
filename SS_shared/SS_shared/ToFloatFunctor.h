#pragma once
#include "IFunctor.h"
class ToFloatFunctor :
	public IFunctor
{
public:
	ToFloatFunctor();
	virtual ~ToFloatFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

