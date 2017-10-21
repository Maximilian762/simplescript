#pragma once
#include "IFunctor.h"

class StringConcat :
	public IFunctor
{
public:
	StringConcat();
	virtual ~StringConcat();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

