#pragma once
#include "IFunctor.h"
class HasVarFunctor :
	public IFunctor
{
public:
	HasVarFunctor();
	virtual ~HasVarFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

