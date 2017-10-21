#pragma once
#include "AST_Expr.h"
#include <vector>
#include <memory>
#include <map>
#include "Table.h"
#include "TableTemplate.h"
#include "API_Collection.h"

class AST_Root :
	public AST_Expr
{
public:
	AST_Root();
	~AST_Root();

	std::vector<SharedExpr>& GetChildExpressions();

	virtual void PrintSelf(int level);

	void CreateTableTemplates(API_Collection &apicollection, std::map<std::string, TableTemplate> &templates);

private:
	void CreateDefaultClasses();

	std::vector<SharedExpr> children;
};

typedef std::shared_ptr<AST_Root> SharedRoot;

