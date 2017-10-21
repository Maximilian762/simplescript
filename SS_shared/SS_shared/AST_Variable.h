#pragma once
#include "AST_Expr.h"
#include <string>
#include <memory>

class Variable;

class AST_Variable :
	public AST_Expr
{
public:
	AST_Variable(std::string _type, std::string _name, std::string _value = "");
	AST_Variable() = delete;
	~AST_Variable();

	virtual void PrintSelf(int level);
	void BecomeReference();
	bool IsReference();

	std::string GetType();
	std::string GetName();
	std::string GetValue();

	bool SameType(Variable &arg);

	virtual bool Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler);
private:
	std::string type;
	std::string name;
	std::string value;
	bool is_reference = false;

};

typedef std::shared_ptr<AST_Variable> SharedVar;