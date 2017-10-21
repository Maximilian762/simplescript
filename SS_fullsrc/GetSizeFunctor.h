#pragma once
#include "IFunctor.h"
class GetSizeFunctor :
	public IFunctor
{
public:
	GetSizeFunctor();
	virtual ~GetSizeFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

