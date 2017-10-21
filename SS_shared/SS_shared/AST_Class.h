#pragma once
#include "AST_Expr.h"
#include "AST_Variable.h"
#include <memory>
#include <vector>
#include "AST_Function.h"
#include "AST_Assignment.h"
#include "Table.h"
#include "TableTemplate.h"
#include "IMsgHandler.h"

class AST_Class :
	public AST_Expr
{
public:
	AST_Class(std::string _classname);
	AST_Class() = delete;
	~AST_Class();

	void AddVariableAssignments(std::vector<SharedExpr>& assignments, IMsgHandler &errhandler);

	std::vector<SharedAssignment>& GetVariableAssignments();
	std::vector<SharedFunction>& GetFunctions();

	virtual void PrintSelf(int level);

	TableTemplate CreateTableTemplate();

	void AddBaseClass(std::string classname);

	std::string GetClassName();

private:
	std::string classname;
	std::vector<std::string> baseclassnames;

	std::vector<SharedAssignment> variable_assignments;
	std::vector<SharedFunction> functions;
};

typedef std::shared_ptr<AST_Class> SharedClass;