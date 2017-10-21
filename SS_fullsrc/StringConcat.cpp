#include "stdafx.h"
#include "StringConcat.h"
#include "Table.h"

StringConcat::StringConcat()
{
}


StringConcat::~StringConcat()
{
}

VarPtr StringConcat::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	std::string result;

	for (auto &arg : args)
	{
		if (!arg) continue;

		result += arg->GetStringRepresentation();
	}

	return VarPtr(new Variable(result));
}
