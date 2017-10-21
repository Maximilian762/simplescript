#pragma once
#include "IFunctor.h"
class SplitToWordsFunctor :
	public IFunctor
{
public:
	SplitToWordsFunctor();
	virtual ~SplitToWordsFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);
};

