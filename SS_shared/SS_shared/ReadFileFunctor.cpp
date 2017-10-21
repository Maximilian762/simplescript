#include "stdafx.h"
#include "ReadFileFunctor.h"
#include "Table.h"
#include <fstream>
#include <string>
#include "SSInterpreter.h"
using namespace std;

ReadFileFunctor::ReadFileFunctor()
{
}


ReadFileFunctor::~ReadFileFunctor()
{
}

VarPtr ReadFileFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	Table *filetable = interp.InstantiateTable("table");
	if (filetable == nullptr) return VarPtr(new Variable(0));

	if (!args.empty() && args[0])
	{
		ifstream file(args[0]->GetStringRepresentation());
		vector<string> filecontents;
		string line;
		while (getline(file, line)) filecontents.push_back(line);

		filetable->AddArray(filecontents);
	}

	return VarPtr(new Variable(filetable));
}
