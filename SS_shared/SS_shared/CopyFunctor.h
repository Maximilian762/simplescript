#pragma once
#include "IFunctor.h"
class CopyFunctor :
	public IFunctor
{
public:
	CopyFunctor();
	virtual ~CopyFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

