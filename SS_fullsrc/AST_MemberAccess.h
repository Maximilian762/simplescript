#pragma once
#include "AST_Expr.h"
#include <string>

class AST_MemberAccess :
	public AST_Expr
{
public:
	AST_MemberAccess(std::string _objname, SharedExpr _member);
	AST_MemberAccess() = delete;
	~AST_MemberAccess();

	virtual void PrintSelf(int level);

	virtual bool Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler);

	void SetCreatingOnRequestStatus(bool _create_at_request);

private:
	std::string objname;
	SharedExpr memberexpr;

	bool create_at_request = false;
};

typedef std::shared_ptr<AST_MemberAccess> SharedMemberAccess;