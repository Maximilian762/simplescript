#pragma once
#include "AST_Expr.h"
#include "Variable.h"
#include <map>

class IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right) = 0;
};
typedef std::shared_ptr<IOperator> SharedOperator;

class AddOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class SubOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class MulOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class DivOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class GreaterOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class LessOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class GreaterOrEqualOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class LessOrEqualOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class EqualOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class NotEqualOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class AndOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class OrOperator : public IOperator
{
public:
	virtual Variable operation(Variable &left, Variable &right);
};

class OperatorMgr
{
public:
	static OperatorMgr& Instance();
	Variable RunOperation(std::string operatorstr, Variable &left, Variable &right);

private:
	OperatorMgr();
	~OperatorMgr();
	void AddDefaultOperators();

	std::map<std::string, SharedOperator> operators;
};

