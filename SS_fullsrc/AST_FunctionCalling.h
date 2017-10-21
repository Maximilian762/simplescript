#pragma once
#include "AST_Expr.h"
#include "Token.h"
#include <vector>
#include <memory>

class AST_FunctionCalling :
	public AST_Expr
{
public:
	AST_FunctionCalling(std::string _callername, std::string _name, std::vector<SharedExpr> &_arguments);
	AST_FunctionCalling() = delete;
	~AST_FunctionCalling();

	std::string GetName();
	std::vector<SharedExpr>& GetArgs();

	virtual void PrintSelf(int level);

	virtual bool Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler);

private:
	std::vector<VarPtr> EvaluateArguments(Table & obj, IMsgHandler &errhandler);

	std::string callername;
	std::string name;
	std::vector<SharedExpr> arguments;
};

typedef std::shared_ptr<AST_FunctionCalling> SharedFuncCalling;