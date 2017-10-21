#pragma once
#include "AST_Expr.h"
#include <string>
#include <vector>
#include "VariableContainer.h"

class SSInterpreter;

class AST_Assignment :
	public AST_Expr
{
public:
	AST_Assignment(SSInterpreter &interp);
	~AST_Assignment();

	void SetVar(SharedExpr var);
	void SetVarValue(SharedExpr val);

	virtual void PrintSelf(int level);

	bool AddNewVarsToVarContainer(Table &obj, bool temp, IMsgHandler &errhandler);

	virtual bool Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler);

private:
	SharedExpr dst;
	SharedExpr value;

	SSInterpreter &interp;
};

typedef std::shared_ptr<AST_Assignment> SharedAssignment;

