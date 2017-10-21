#pragma once
#include <memory>
#include <iostream>
#include <vector>
#include "IMsgHandler.h"

class Variable;
class Table;
typedef std::shared_ptr<Variable> VarPtr;

enum class AST_Type
{
	BaseExpr,
	Assignment,
	Binary,
	Class,
	For,
	Function,
	FunctionCalling,
	If,
	MemberAccess,
	Number,
	Return,
	Root,
	StringLiteral,
	Variable
};

class AST_Expr
{
public:
	AST_Expr();
	virtual ~AST_Expr();

	virtual void PrintSelf(int level);

	AST_Type GetType();
	bool Is(AST_Type t);

	virtual bool Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler);

protected:
	void PrintAlignment(int level);
	void SetType(AST_Type t);

private:
	AST_Type _type = AST_Type::BaseExpr;
};

typedef std::shared_ptr<AST_Expr> SharedExpr;