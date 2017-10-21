#pragma once
#include "AST_Expr.h"
#include "AST_Assignment.h"
#include "AST_BinaryExpr.h"

class AST_For :
	public AST_Expr
{
public:
	AST_For(SharedAssignment _init_assignm, SharedBinExpr _cmp, SharedAssignment _final_assignm, std::vector<SharedExpr>& _body);
	AST_For() = delete;
	~AST_For();

	virtual void PrintSelf(int level);

	virtual bool Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler);

private:
	SharedAssignment initial_assignment;
	SharedBinExpr comparison;
	SharedAssignment final_assignment;

	std::vector<SharedExpr> body;
};

typedef std::shared_ptr<AST_For> SharedFor;