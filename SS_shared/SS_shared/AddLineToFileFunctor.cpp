#include "stdafx.h"
#include "AddLineToFileFunctor.h"
#include <fstream>
using namespace std;

AddLineToFileFunctor::AddLineToFileFunctor()
{
}


AddLineToFileFunctor::~AddLineToFileFunctor()
{
}

VarPtr AddLineToFileFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	if (args.size() == 2 && args[0] && args[1])
	{
		ofstream file(args[0]->GetStringRepresentation(), ios::app);
		file << args[1]->GetStringRepresentation();
	}
	else errhandler.OnMsg("Adding line to file functor expected args: <filename> <line>");

	return VarPtr(new Variable(0));
}
