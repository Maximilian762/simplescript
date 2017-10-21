#pragma once
#include "Variable.h"
#include "IMsgHandler.h"

class SSInterpreter;

class IFunctor
{
public:
	IFunctor();
	virtual ~IFunctor();

	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler) = 0;
};

typedef std::shared_ptr<IFunctor> SharedFunctor;
