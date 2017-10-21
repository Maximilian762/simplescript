#pragma once
#include "AST_Expr.h"
#include <memory>
#include <string>
#include "Variable.h"

class AST_Number :
	public AST_Expr
{
public:
	AST_Number(std::string n);
	AST_Number() = delete;
	~AST_Number();

	std::string GetNumberStr();

	virtual void PrintSelf(int level);

	virtual bool Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler);
private:
	std::string number;
};

typedef std::shared_ptr<AST_Number> SharedNumber;