#pragma once
#include "AST_Expr.h"
#include <vector>

class ASt_If :
	public AST_Expr
{
public:
	ASt_If();
	~ASt_If();

	void SetCondition(SharedExpr _ast_cond);
	void SetCode(std::vector<SharedExpr> _code);
	void SetElseCode(std::vector<SharedExpr> _code);

	virtual void PrintSelf(int level);

	virtual bool Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler);

private:
	SharedExpr ast_cond;
	std::vector<SharedExpr> code;
	std::vector<SharedExpr> else_code;
};

typedef std::shared_ptr<ASt_If> SharedIf;