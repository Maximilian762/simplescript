#pragma once
#include "AST_Expr.h"
#include <string>

class AST_StringLiteral :
	public AST_Expr
{
public:
	AST_StringLiteral(std::string val);
	AST_StringLiteral() = delete;
	~AST_StringLiteral();

	virtual void PrintSelf(int level);

	virtual bool Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler);
private:
	std::string value;
};

typedef std::shared_ptr<AST_StringLiteral> SharedStringLiteral;