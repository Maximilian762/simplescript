#include "stdafx.h"
#include "OperatorMgr.h"


OperatorMgr & OperatorMgr::Instance()
{
	static OperatorMgr opmgr;
	return opmgr;
}

Variable OperatorMgr::RunOperation(std::string operatorstr, Variable &left, Variable &right)
{
	auto found = operators.find(operatorstr);
	if (found != operators.end())
	{
		return found->second->operation(left, right);
	}
	else return Variable();
}

OperatorMgr::OperatorMgr()
{
	AddDefaultOperators();
}

OperatorMgr::~OperatorMgr()
{
}

void OperatorMgr::AddDefaultOperators()
{
	operators["+"] = SharedOperator(new AddOperator);
	operators["-"] = SharedOperator(new SubOperator);
	operators["*"] = SharedOperator(new MulOperator);
	operators["/"] = SharedOperator(new DivOperator);
	operators[">"] = SharedOperator(new GreaterOperator);
	operators["<"] = SharedOperator(new LessOperator);
	operators[">="] = SharedOperator(new GreaterOrEqualOperator);
	operators["<="] = SharedOperator(new LessOrEqualOperator);
	operators["=="] = SharedOperator(new EqualOperator);
	operators["!="] = SharedOperator(new NotEqualOperator);
	operators["&&"] = SharedOperator(new AndOperator);
	operators["||"] = SharedOperator(new OrOperator);
}

Variable AddOperator::operation(Variable &left, Variable &right)
{
	if (left.IntOrFloat() && right.IntOrFloat())
	{
		if (left.Type() == VarType::Int && right.Type() == VarType::Int)
		{
			return Variable(int(left.GetIntRef() + right.GetIntRef()));
		}
		else return Variable(float(left.GetNumber() + right.GetNumber()));
	}
	else return Variable();
}

Variable GreaterOperator::operation(Variable & left, Variable & right)
{
	if (left.IntOrFloat() && right.IntOrFloat())
	{
		if (left.Type() == VarType::Int && right.Type() == VarType::Int)
		{
			return Variable(int(left.GetIntRef() > right.GetIntRef()));
		}
		else return Variable(int(left.GetNumber() > right.GetNumber()));
	}
	else return Variable();
}

Variable SubOperator::operation(Variable & left, Variable & right)
{
	if (left.IntOrFloat() && right.IntOrFloat())
	{
		if (left.Type() == VarType::Int && right.Type() == VarType::Int)
		{
			return Variable(int(left.GetIntRef() - right.GetIntRef()));
		}
		else return Variable(float(left.GetNumber() - right.GetNumber()));
	}
	else return Variable();
}

Variable MulOperator::operation(Variable & left, Variable & right)
{
	if (left.IntOrFloat() && right.IntOrFloat())
	{
		if (left.Type() == VarType::Int && right.Type() == VarType::Int)
		{
			return Variable(int(left.GetIntRef() * right.GetIntRef()));
		}
		else return Variable(float(left.GetNumber() * right.GetNumber()));
	}
	else return Variable();
}

Variable DivOperator::operation(Variable & left, Variable & right)
{
	if (left.IntOrFloat() && right.IntOrFloat())
	{
		if (left.Type() == VarType::Int && right.Type() == VarType::Int)
		{
			if (right.GetIntRef() == 0) return Variable(0);
			else return Variable(int(left.GetIntRef() / right.GetIntRef()));
		}
		else return Variable(float(left.GetNumber() / right.GetNumber()));
	}
	else return Variable();
}

Variable LessOperator::operation(Variable & left, Variable & right)
{
	if (left.IntOrFloat() && right.IntOrFloat())
	{
		if (left.Type() == VarType::Int && right.Type() == VarType::Int)
		{
			return Variable(int(left.GetIntRef() < right.GetIntRef()));
		}
		else return Variable(int(left.GetNumber() < right.GetNumber()));
	}
	else return Variable();
}

Variable GreaterOrEqualOperator::operation(Variable & left, Variable & right)
{
	if (left.IntOrFloat() && right.IntOrFloat())
	{
		if (left.Type() == VarType::Int && right.Type() == VarType::Int)
		{
			return Variable(int(left.GetIntRef() >= right.GetIntRef()));
		}
		else return Variable(int(left.GetNumber() >= right.GetNumber()));
	}
	else return Variable();
}

Variable LessOrEqualOperator::operation(Variable & left, Variable & right)
{
	if (left.IntOrFloat() && right.IntOrFloat())
	{
		if (left.Type() == VarType::Int && right.Type() == VarType::Int)
		{
			return Variable(int(left.GetIntRef() <= right.GetIntRef()));
		}
		else return Variable(int(left.GetNumber() <= right.GetNumber()));
	}
	else return Variable();
}

Variable EqualOperator::operation(Variable & left, Variable & right)
{
	if (left.IntOrFloat() && right.IntOrFloat())
	{
		if (left.Type() == VarType::Int && right.Type() == VarType::Int)
		{
			return Variable(int(left.GetIntRef() == right.GetIntRef()));
		}
		else
		{
			return Variable(int(abs(left.GetNumber() - right.GetNumber()) < 0.00001));
		}
	}
	else if (left.Type() == VarType::String)
	{
		return Variable(int(left.GetStringRef() == right.GetStringRef()));
	}
	else return Variable();
}

Variable NotEqualOperator::operation(Variable & left, Variable & right)
{
	if (left.IntOrFloat() && right.IntOrFloat())
	{
		if (left.Type() == VarType::Int && right.Type() == VarType::Int)
		{
			return Variable(int(left.GetIntRef() != right.GetIntRef()));
		}
		else
		{
			return Variable(int(abs(left.GetNumber() - right.GetNumber()) > 0.00001));
		}
	}
	else if (left.Type() == VarType::String)
	{
		return Variable(int(left.GetStringRef() != right.GetStringRef()));
	}
	else return Variable();
}

Variable AndOperator::operation(Variable & left, Variable & right)
{
	return left.GetIntRef() && right.GetIntRef();
}

Variable OrOperator::operation(Variable & left, Variable & right)
{
	return left.GetIntRef() || right.GetIntRef();
}
