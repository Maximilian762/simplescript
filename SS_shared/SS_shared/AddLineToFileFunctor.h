#pragma once
#include "IFunctor.h"
class AddLineToFileFunctor :
	public IFunctor
{
public:
	AddLineToFileFunctor();
	virtual ~AddLineToFileFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

