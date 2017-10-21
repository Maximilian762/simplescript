#include "stdafx.h"
#include "AST_Number.h"
#include <iostream>
using namespace std;

AST_Number::AST_Number(std::string n)
{
	SetType(AST_Type::Number);
	number = n;
}

AST_Number::~AST_Number()
{
}

std::string AST_Number::GetNumberStr()
{
	return number;
}

void AST_Number::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << (number.empty() ? "<emptynumber>" : number) << endl;
}

bool AST_Number::Evaluate(Table & obj, VarPtr &return_var, IMsgHandler &errhandler)
{
	Variable *v;
	if (number.find('.') == string::npos)
	{
		v = new Variable(stoi(number));
	}
	else v = new Variable(stof(number));
		
	return_var = VarPtr(v);
	return true;
}
