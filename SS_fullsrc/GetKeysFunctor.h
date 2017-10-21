#pragma once
#include "IFunctor.h"
class GetKeysFunctor :
	public IFunctor
{
public:
	GetKeysFunctor();
	virtual ~GetKeysFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

