#pragma once
#include "IFunctor.h"
class ReadFileFunctor :
	public IFunctor
{
public:
	ReadFileFunctor();
	virtual ~ReadFileFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

