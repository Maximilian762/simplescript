#include "stdafx.h"
#include "AST_Root.h"
#include "AST_Class.h"
using namespace std;

AST_Root::AST_Root()
{
	SetType(AST_Type::Root);
	CreateDefaultClasses();
}


AST_Root::~AST_Root()
{
}

std::vector<SharedExpr>& AST_Root::GetChildExpressions()
{
	return children;
}

void AST_Root::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "[AST ROOT]" << endl;

	for (auto &expr : children)
	{
		if (expr) expr->PrintSelf(level + 1);
		else
		{
			PrintAlignment(level + 1);
			cout << "<emptyexpr>" << endl;
		}
	}

	PrintAlignment(level);
	cout << "[/AST ROOT]" << endl;
}

void AST_Root::CreateTableTemplates(API_Collection &apicollection, std::map<std::string, TableTemplate> &templates)
{
	for (auto &expr : children)
	{
		if (auto astclass = dynamic_pointer_cast<AST_Class>(expr))
		{
			if (templates.find(astclass->GetClassName()) == templates.end())
			{
				TableTemplate templ = astclass->CreateTableTemplate();

				if (!templ.GetName().empty())
				{
					apicollection.HandleClassDefinitionTemplate(templ);
					templates[templ.GetName()] = templ;
				}
			}
		}
	}
}

void AST_Root::CreateDefaultClasses()
{
	SharedClass ss(new AST_Class("ss"));
	children.push_back(ss);

	SharedClass table(new AST_Class("table"));
	children.push_back(table);
}
