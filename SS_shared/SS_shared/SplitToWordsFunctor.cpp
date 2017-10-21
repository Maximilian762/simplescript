#include "stdafx.h"
#include "SplitToWordsFunctor.h"
#include "common.h"
#include "Table.h"
#include "SSInterpreter.h"

using namespace std;

SplitToWordsFunctor::SplitToWordsFunctor()
{
}


SplitToWordsFunctor::~SplitToWordsFunctor()
{
}

VarPtr SplitToWordsFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	Table *t = interp.InstantiateTable("table");
	if (t == nullptr) return VarPtr(new Variable(0));


	if (args.size() == 1 && args[0])
	{
		auto words = split(args[0]->GetStringRepresentation(), ' ');
		t->AddArray(words);
	}
	else errhandler.OnMsg("SplitToWords functor expects one argument <string>");

	return VarPtr(new Variable(t));
}
