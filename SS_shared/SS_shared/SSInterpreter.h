#pragma once
#include <vector>
#include <string>
#include "Token.h"
#include "AST_Root.h"
#include "AST_Class.h"
#include "AST_Function.h"
#include <set>
#include <queue>
#include <map>
#include "Table.h"
#include "Variable.h"
#include "TableTemplate.h"
#include "API_Collection.h"
#include "IMsgHandler.h"

class SSInterpreter
{
public:
	SSInterpreter();
	SSInterpreter(std::string filepath);
	~SSInterpreter();

	void LoadFromFile(std::string filepath);
	void LoadFromFile(std::string filepath, std::vector<API_Collection> api_collections);

	void LoadFromText(std::vector<std::string> text);
	void LoadFromText(std::vector<std::string> text, std::vector<API_Collection> api_collections);

	Table* InstantiateTable(std::string type_name);

private:
	std::vector<std::string> SplitCodeToWords(std::string code);
	std::vector<Token> Tokenize(const std::vector<std::string> &words, int curline);

	void MakeAST(std::queue<Token> &tokens);

	//Should be called when class header and initial '{' already extracted;
	//extracts everything from beginning of queue, stops after final '}' is extracted
	void LoadClassBody(SharedClass astclass, std::queue<Token> &tokens);

	//Should be called when function type, name and initial '(' already extracted;
	//extracts everything from beginning of queue, stops after final '}' is extracted
	bool LoadFunction(SharedFunction astfunc, std::queue<Token> &tokens);

	bool ParseCode(std::vector<SharedExpr> &thecode, std::queue<Token> &tokens);

	bool TryToParseIf(std::vector<SharedExpr> &thecode, std::queue<Token> &tokens);
	bool TryToParseFor(std::vector<SharedExpr> &thecode, std::queue<Token> &tokens);
	bool TryToParseAssignment(std::vector<SharedExpr> &thecode, std::queue<Token> &tokens, bool finalsemicolon_doesntwork);
	bool TryToParseMemberAssignment(std::vector<SharedExpr> &thecode, std::queue<Token> &tokens);
	bool TryToParseReturn(std::vector<SharedExpr> &thecode, std::queue<Token> &tokens);
	bool TryToParseFuncCall(std::vector<SharedExpr> &thecode, std::queue<Token> &tokens, bool finalsemicolon);

	bool ParseCommaSeparatedOperands(std::vector<SharedExpr> &operands, std::queue<Token>& tokens);

	void RegisterTypename(std::string thetypename);
	bool IsTypename(std::string s);

	//should be only binary expr in that queue
	SharedExpr ParseBinaryExpr(std::queue<Token> &tokens);
	SharedExpr ParseNextOperand(std::queue<Token> &tokens);

	void RegisterCustomDataTypesFromCode(std::vector<Token> &tokens);

	Table* InstantiateTableWithParsedArgs(std::string type_name, std::vector<Variable> args);

	void RegisterDefaultTypenames();
	void RegisterOperators();
	bool IsOperator(Token &t);
	int GetPrecedence(Token &t);

	std::string GetFileNameFromPath(std::string path);

	void SetDefaultApiCollection();
	void ReportTokenErrorToHandler(Token t, std::string message);

private:
	std::set<std::string> typenames;
	std::set<std::string> files_parsed;
	std::map<std::string, int> operators; //symbol:precedence
	std::map<std::string, TableTemplate> tabletemplates;
	SharedRoot ast_root;
	IMsgHandler errhandler;

	API_Collection apicollection;
};


