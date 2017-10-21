#pragma once
#include "IFunctor.h"
class DeleteVarFunctor :
	public IFunctor
{
public:
	DeleteVarFunctor();
	virtual ~DeleteVarFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

