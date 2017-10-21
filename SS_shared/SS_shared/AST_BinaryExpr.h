#pragma once
#include "AST_Expr.h"
#include "Token.h"
#include <memory>

class AST_BinaryExpr :
	public AST_Expr
{
public:
	AST_BinaryExpr(Token _optoken, SharedExpr _left = nullptr, SharedExpr _right = nullptr);
	AST_BinaryExpr() = delete;
	virtual ~AST_BinaryExpr();

	void SetOperands(SharedExpr _left, SharedExpr _right);

	virtual void PrintSelf(int level);

	virtual bool Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler);
private:
	Token optok;
	SharedExpr left;
	SharedExpr right;
};

typedef std::shared_ptr<AST_BinaryExpr> SharedBinExpr;