#include "stdafx.h"
#include "API_Collection.h"

using namespace std;

API_Collection::API_Collection()
{
}


API_Collection::~API_Collection()
{
}

void API_Collection::AddMethod(std::string classname, std::string methodname, SharedFunctor f, SSInterpreter &interpreter)
{
	SharedFunction newfunc(new AST_Function(interpreter, methodname, f));

	auto found = methods.find(classname);
	if (found != methods.end())
	{
		found->second.push_back(newfunc);
	}
	else
	{
		methods[classname] = { newfunc };
	}
}

void API_Collection::HandleClassDefinitionTemplate(TableTemplate &templ)
{
	auto found = methods.find(templ.GetName());

	if (found != methods.end())
	{
		for (auto &func : found->second) templ.GetMethodContainer().AddMethod(func->GetName(), func);
	}

	for (auto it = methods.begin(); it != methods.end(); ++it)
	{
		if (it->first == "*")
		{
			for (auto &func : it->second) templ.GetMethodContainer().AddMethod(func->GetName(), func);
		}
	}
}

void API_Collection::AppendOtherApi(API_Collection & other)
{
	for (auto it = other.methods.begin(); it != other.methods.end(); ++it)
	{
		string classname = it->first;

		auto found = methods.find(classname);
		if (found != methods.end())
		{
			found->second.insert(found->second.end(), it->second.begin(), it->second.end());
		}
		else methods[classname] = it->second;
	}
}

std::string API_Collection::InfoToString()
{
	string result;

	for (auto it = methods.begin(); it != methods.end(); ++it)
	{
		result += "CLASS " + it->first + "\n";
		for (auto &func : it->second)
		{
			result += "\t" + (func?func->GetName():"<error: emptyfunc>") + "\n";
		}
	}

	return result;
}
