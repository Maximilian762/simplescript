#include "stdafx.h"
#include "MethodContainer.h"


MethodContainer::MethodContainer()
{
}


MethodContainer::~MethodContainer()
{
}

bool MethodContainer::HasMethod(std::string name)
{
	return methods.find(name) != methods.end();
}

void MethodContainer::AddMethod(std::string name, SharedFunction meth)
{
	methods[name] = meth;
}

SharedFunction MethodContainer::GetMethod(std::string name)
{
	auto found = methods.find(name);
	if (found != methods.end()) return found->second;
	else return nullptr;
}

std::string MethodContainer::ToString()
{
	std::string info = "methods:";
	for (auto it = methods.begin(); it != methods.end(); it++)
	{
		info += "\n";
		info += it->second->ToString();
	}
	return info;
}

void MethodContainer::Append(MethodContainer & other)
{
	for (auto it = other.methods.begin(); it != other.methods.end(); ++it)
	{
		methods[it->first] = it->second;
	}
}
