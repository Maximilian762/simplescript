#pragma once
#include "IFunctor.h"

class ToIntFunctor :
	public IFunctor
{
public:
	ToIntFunctor();
	virtual ~ToIntFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

