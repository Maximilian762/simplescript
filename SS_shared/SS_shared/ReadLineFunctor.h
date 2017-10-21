#pragma once
#include "IFunctor.h"
class ReadLineFunctor :
	public IFunctor
{
public:
	ReadLineFunctor();
	virtual ~ReadLineFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

