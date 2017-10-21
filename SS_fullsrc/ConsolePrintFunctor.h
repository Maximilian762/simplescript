#pragma once
#include "IFunctor.h"
class ConsolePrintFunctor :
	public IFunctor
{
public:
	ConsolePrintFunctor();
	virtual ~ConsolePrintFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

