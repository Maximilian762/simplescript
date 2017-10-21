#include "stdafx.h"
#include "ConsolePrintFunctor.h"
#include <iostream>
using namespace std;

ConsolePrintFunctor::ConsolePrintFunctor()
{
}


ConsolePrintFunctor::~ConsolePrintFunctor()
{
}

VarPtr ConsolePrintFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	for (auto &arg : args)
	{
		if (arg) cout << arg->GetStringRepresentation();
	}
	return VarPtr(new Variable(0));
}
