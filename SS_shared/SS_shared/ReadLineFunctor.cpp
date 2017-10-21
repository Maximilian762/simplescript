#include "stdafx.h"
#include "ReadLineFunctor.h"
#include <string>
#include <iostream>
using namespace std;

ReadLineFunctor::ReadLineFunctor()
{
}


ReadLineFunctor::~ReadLineFunctor()
{
}

VarPtr ReadLineFunctor::Call(SSInterpreter & interp, Table & obj, std::vector<VarPtr> args, IMsgHandler & errhandler)
{
	string line;
	cin >> line;

	return VarPtr(new Variable(line));
}
