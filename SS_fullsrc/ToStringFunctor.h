#pragma once
#include "IFunctor.h"
class ToStringFunctor :
	public IFunctor
{
public:
	ToStringFunctor();
	virtual ~ToStringFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

