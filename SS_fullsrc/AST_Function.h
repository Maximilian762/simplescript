#pragma once
#include "AST_Expr.h"
#include "AST_Variable.h"
#include <memory>
#include <vector>
#include "Variable.h"
#include "IFunctor.h"

class SSInterpreter;

class AST_Function :
	public AST_Expr
{
public:
	AST_Function(SSInterpreter &interpreter, std::string _name, bool _returns_ref = false);
	AST_Function(SSInterpreter &interpreter, std::string _name, SharedFunctor _functor);
	AST_Function() = delete;
	~AST_Function();

	std::vector<SharedExpr>& GetCode();
	void SetCode(std::vector<SharedExpr> thecode);
	void SetArguments(std::vector<SharedVar> theargs);
	virtual void PrintSelf(int level);

	std::string GetName();

	VarPtr Call(Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler);

	std::string ToString();

private:
	VarPtr ReturnValue(VarPtr rawresult);

	std::string name;
	bool returns_ref = false;

	std::vector<SharedExpr> code;
	std::vector<SharedVar> args;

	SharedFunctor functor;
	SSInterpreter &interp;
};

typedef std::shared_ptr<AST_Function> SharedFunction;