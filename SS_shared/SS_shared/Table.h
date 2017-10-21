#pragma once
#include "LayeredVarContainer.h"
#include "MethodContainer.h"

class Table
{
public:
	Table(std::string _name);
	Table();
	~Table();

	LayeredVarContainer& GetVarContainer();
	MethodContainer& GetMethodContainer();

	std::string GetName();
	std::string ToString();

	Variable CallMethod(std::string methodname);
	template <typename T1>
	Variable CallMethod(std::string methodname, T1 arg1);
	template <typename T1, typename T2>
	Variable CallMethod(std::string methodname, T1 arg1, T2 arg2);
	template <typename T1, typename T2, typename T3>
	Variable CallMethod(std::string methodname, T1 arg1, T2 arg2, T3 arg3);
	template <typename T1, typename T2, typename T3, typename T4>
	Variable CallMethod(std::string methodname, T1 arg1, T2 arg2, T3 arg3, T4 arg4);
	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	Variable CallMethod(std::string methodname, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	Variable CallMethod(std::string methodname, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6);
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	Variable CallMethod(std::string methodname, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7);

	template <typename T>
	void AddArray(std::vector<T> &arr);


private:
	Variable _CallMethodWithParsedArgs(std::string methodname, std::vector<Variable> args);
	std::string name;

	LayeredVarContainer vars;
	MethodContainer methods;
};

template<typename T1>
inline Variable Table::CallMethod(std::string methodname, T1 arg1)
{
	return _CallMethodWithParsedArgs(methodname, { Variable(arg1) });
}

template<typename T1, typename T2>
inline Variable Table::CallMethod(std::string methodname, T1 arg1, T2 arg2)
{
	return _CallMethodWithParsedArgs(methodname, { Variable(arg1), Variable(arg2) });
}

template<typename T1, typename T2, typename T3>
inline Variable Table::CallMethod(std::string methodname, T1 arg1, T2 arg2, T3 arg3)
{
	return _CallMethodWithParsedArgs(methodname, { Variable(arg1), Variable(arg2), Variable(arg3) });
}

template<typename T1, typename T2, typename T3, typename T4>
inline Variable Table::CallMethod(std::string methodname, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
{
	return _CallMethodWithParsedArgs(methodname, { Variable(arg1), Variable(arg2), Variable(arg3), Variable(arg4) });
}

template<typename T1, typename T2, typename T3, typename T4, typename T5>
inline Variable Table::CallMethod(std::string methodname, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
{
	return _CallMethodWithParsedArgs(methodname, { Variable(arg1), Variable(arg2), Variable(arg3), Variable(arg4), Variable(arg5) });
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
inline Variable Table::CallMethod(std::string methodname, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
{
	return _CallMethodWithParsedArgs(methodname, { Variable(arg1), Variable(arg2), Variable(arg3), Variable(arg4), Variable(arg5), Variable(arg6) });
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
inline Variable Table::CallMethod(std::string methodname, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
{
	return _CallMethodWithParsedArgs(methodname, {Variable(arg1), Variable(arg2), Variable(arg3), Variable(arg4), Variable(arg5), Variable(arg6), Variable(arg7)});
}

template<typename T>
inline void Table::AddArray(std::vector<T>& arr)
{
	int beginindex = vars.GetRootContainer().GetLastNumerableVarName() + 1;
	for (int i = 0; i < arr.size(); ++i)
	{
		vars.GetRootContainer().AddVar(std::to_string(beginindex + i), VarPtr(new Variable(arr[i])));
	}
}
