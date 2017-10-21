#include "stdafx.h"
#include "SSInterpreter.h"
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <queue>
#include "AST_Class.h"
#include "AST_Variable.h"
#include "ASt_If.h"
#include "AST_Assignment.h"
#include <stack>
#include "AST_Number.h"
#include "AST_FunctionCalling.h"
#include "AST_BinaryExpr.h"
#include "AST_For.h"
#include "AST_Return.h"
#include "AST_MemberAccess.h"
#include "AST_StringLiteral.h"
#include <ctime>
#include "HasVarFunctor.h"
#include "StringConcat.h"
#include "AddLineToFileFunctor.h"
#include "ConsolePrintFunctor.h"
#include "CopyFunctor.h"
#include "ReadFileFunctor.h"
#include "SplitToWordsFunctor.h"
#include "ReadLineFunctor.h"
#include "GetSizeFunctor.h"
#include "DeleteVarFunctor.h"
#include "ToFloatFunctor.h"
#include "ToStringFunctor.h"
#include "ToIntFunctor.h"
#include "GetKeysFunctor.h"
#include "common.h"
using namespace std;

SSInterpreter::SSInterpreter()
{
	ast_root = SharedRoot(new AST_Root);
	RegisterDefaultTypenames();
	RegisterOperators();
	SetDefaultApiCollection();
}

SSInterpreter::SSInterpreter(std::string filepath)
{
	ast_root = SharedRoot(new AST_Root);
	RegisterDefaultTypenames();
	RegisterOperators();
	SetDefaultApiCollection();

	LoadFromFile(filepath);
}


SSInterpreter::~SSInterpreter()
{
}

void SSInterpreter::LoadFromFile(std::string filepath)
{
	LoadFromFile(filepath, std::vector<API_Collection>());
}

void SSInterpreter::LoadFromFile(std::string filepath, std::vector<API_Collection> api_collections)
{
	std::vector<std::string> text;
	ifstream file(filepath);
	string line;
	while (getline(file, line)) text.push_back(line);
	LoadFromText(text, api_collections);
}

void SSInterpreter::LoadFromText(std::vector<std::string> text)
{
	LoadFromText(text, std::vector<API_Collection>());
}

void SSInterpreter::LoadFromText(std::vector<std::string> text, std::vector<API_Collection> api_collections)
{
	clock_t loading_start = clock();

	for (auto &api : api_collections) apicollection.AppendOtherApi(api);

	vector<Token> tokens;

	bool parsingincludes = true;

	int curline = 0;
	for (auto &line : text)
	{
		curline++;
		if (line.empty()) continue;

		auto words = SplitCodeToWords(line);
		auto words_simple = split(line, ' ');

		if (parsingincludes && !words_simple.empty() && words_simple[0] == "include")
		{
			string include_filepath;
			for (int i = 1; i < words_simple.size(); ++i) include_filepath += words_simple[i];

			string filename = GetFileNameFromPath(include_filepath);
			if (files_parsed.find(filename) == files_parsed.end())
			{
				files_parsed.emplace(filename);
				LoadFromFile(include_filepath);
			}
			continue;
		}
		else parsingincludes = false;

		auto line_tokens = Tokenize(words, curline);

		tokens.insert(tokens.end(), line_tokens.begin(), line_tokens.end());
	}

	RegisterCustomDataTypesFromCode(tokens);

	queue<Token> tokens_queue;
	for (auto &t : tokens) tokens_queue.push(t);

	MakeAST(tokens_queue);

	ast_root->CreateTableTemplates(apicollection, tabletemplates);

	clock_t loading_end = clock();

	//ast_root->PrintSelf(0);

	cout << "Loading completed. Time = " << loading_end - loading_start << endl;
}

std::vector<std::string> SSInterpreter::SplitCodeToWords(std::string code)
{
	vector<string> words;
	string curword;

	bool saving_stringliteral = false;

	for (int i = 0; i < code.length(); ++i)
	{
		if (code[i] == '\\') continue;

		if (code[i] == '"' && (i>0?code[i-1] != '\\' : true))
		{
			saving_stringliteral = !saving_stringliteral;

			if (!saving_stringliteral)
			{
				words.push_back("\"" + curword + "\"");
				curword = "";
			}

			continue;
		}

		if (saving_stringliteral && code[i] == 'n' && i > 0 && code[i - 1] == '\\')
		{
			curword += '\n';
			continue;
		}

		if ((isalnum(code[i]) || code[i] == '_')
			|| 
			(code[i] == '.' && (i+1) < code.length() && i > 0 && isdigit(code[i-1]) && isdigit(code[i+1]))
			||
			saving_stringliteral)
		{
			curword += code[i];
		}
		else
		{
			if (!curword.empty())
			{
				words.push_back(curword);
				curword = "";
			}

			if (!isspace(code[i]))
			{
				if (i < code.length() - 1)
				{
					string twochars_operator = code.substr(i, 2);

					if (operators.find(twochars_operator) != operators.end())
					{
						words.push_back(twochars_operator);
						++i;
						continue;
					}
				}

				words.push_back(string(1, code[i]));
			}
		}
	}

	if (!curword.empty()) words.push_back(curword);

	return words;
}

std::vector<Token> SSInterpreter::Tokenize(const std::vector<std::string>& words, int curline)
{
	std::vector<Token> tokens;
	for (auto &w : words) 
	{
		Token t(w);
		t.SetLine(curline);
		tokens.push_back(t);
	}
	return tokens;
}

void SSInterpreter::MakeAST(queue<Token> &tokens)
{
	while (!tokens.empty())
	{
		Token classtok = tokens.front(); tokens.pop();
		
		if (classtok.Type() == TokenType::Identifier && classtok.Content() == "table")
		{
			Token classnametok = tokens.front(); tokens.pop();
			vector<string> baseclassnames;

			if (classnametok.Type() == TokenType::Identifier)
			{
				Token potential_semicolon = tokens.front();
				if (potential_semicolon.Content() == ":")
				{
					tokens.pop();
					while (!tokens.empty())
					{
						Token baseclass_or_brace = tokens.front();
						if (baseclass_or_brace.Content() != "{")
						{
							tokens.pop();
							baseclassnames.push_back(baseclass_or_brace.Content());
						}
						else break;
					}
				}


				Token openbrace = tokens.front(); tokens.pop();

				if (openbrace.Type() == TokenType::Symbol && openbrace.Content() == "{")
				{
					SharedClass astclass(new AST_Class(classnametok.Content()));
					for (string base : baseclassnames) astclass->AddBaseClass(base);

					LoadClassBody(astclass, tokens);

					ast_root->GetChildExpressions().push_back(astclass);
				}
				else
				{
					ReportTokenErrorToHandler(openbrace, "Expected {");
				}
			}
			else
			{
				ReportTokenErrorToHandler(classnametok, "Expected classname");
			}
		}
		else
		{
			ReportTokenErrorToHandler(classtok, "Expected key word 'table'");
		}
	}
}

void SSInterpreter::LoadClassBody(SharedClass astclass, std::queue<Token>& tokens)
{
	std::vector<SharedExpr> allvariables;

	std::queue<Token> ss_var;
	ss_var.push(Token("ss"));
	ss_var.push(Token("ss"));
	ss_var.push(Token(";"));
	TryToParseAssignment(allvariables, ss_var, true);

	while (!tokens.empty())
	{
		while (TryToParseAssignment(allvariables, tokens, true))
		{
		}

		Token typetok = tokens.front(); tokens.pop();

		if (typetok.Content() == "}") break;

		if (typetok.Type() == TokenType::Identifier)
		{
			if (IsTypename(typetok.Content()))
			{
				bool returns_reference = false;
				Token potential_ampersand = tokens.front();
				if (potential_ampersand.Is(TokenType::Symbol, "&"))
				{
					tokens.pop();
					returns_reference = true;
				}

				Token name = tokens.front(); tokens.pop();

				if (name.Type() == TokenType::Identifier)
				{
					Token next = tokens.front(); tokens.pop();

					if (next.Type() == TokenType::Symbol)
					{
						if (next.Content() == "(") //its function definition
						{
							SharedFunction func(new AST_Function(*this, name.Content(), returns_reference));
							bool succ = LoadFunction(func, tokens);

							if(succ) astclass->GetFunctions().push_back(func);
						}
					}
				}
				else
				{
					ReportTokenErrorToHandler(name, "Expected name identifier");
				}
			}
			else
			{
				ReportTokenErrorToHandler(typetok, typetok.Content() + " is not a type name");
			}
		}
		else
		{
			ReportTokenErrorToHandler(typetok, typetok.Content() + " is not a type name");
		}
	}

	astclass->AddVariableAssignments(allvariables, errhandler);
}

bool SSInterpreter::LoadFunction(SharedFunction astfunc, std::queue<Token>& tokens)
{
	std::vector<SharedVar> args;

	//collecting args
	while (!tokens.empty())
	{
		Token first = tokens.front(); tokens.pop();

		if (first.Content() == ")")
		{
			Token brace = tokens.front(); tokens.pop();
			if (brace.Content() != "{")
			{
				ReportTokenErrorToHandler(brace, "Expected '{'");
				return false;
			}
			break;
		}
		else if (first.Type() == TokenType::Identifier)
		{
			bool is_reference = false;
			Token argname = tokens.front(); tokens.pop();
			if (argname.Is(TokenType::Symbol, "&"))
			{
				is_reference = true;
				argname = tokens.front(); tokens.pop();
			}

			if (argname.Type() == TokenType::Identifier)
			{
				//ALL GOOD
				SharedVar var(new AST_Variable(first.Content(), argname.Content()));
				if (is_reference) var->BecomeReference();
				args.push_back(var);
			}
			else
			{
				ReportTokenErrorToHandler(argname, argname.Content() + " is not identifier");
				return false;
			}
		}
	}

	std::vector<SharedExpr> thecode;
	bool success = ParseCode(thecode, tokens);

	Token brace = tokens.front(); tokens.pop();

	if (brace.Content() != "}")
	{
		ReportTokenErrorToHandler(brace, "Expected '}'");
		return false;
	}

	if (success)
	{
		astfunc->SetCode(thecode);
		astfunc->SetArguments(args);
		return true;
	}
	else
	{
		errhandler.OnMsg("Couldnt parse code of function");
		return false;
	}
}

bool SSInterpreter::ParseCode(std::vector<SharedExpr> &thecode, std::queue<Token>& tokens)
{
	//collecting body
	while (!tokens.empty())
	{
		if (tokens.front().Content() == "}")
		{
			return true;
		}

		std::vector<SharedExpr> nextcodeelement;
		if (TryToParseAssignment(nextcodeelement, tokens, true) ||
			TryToParseIf(nextcodeelement, tokens) ||
			TryToParseFuncCall(nextcodeelement, tokens, true) ||
			TryToParseFor(nextcodeelement, tokens) ||
			TryToParseReturn(nextcodeelement, tokens) ||
			TryToParseMemberAssignment(nextcodeelement, tokens))
		{
			thecode.insert(thecode.end(), nextcodeelement.begin(), nextcodeelement.end());
		}
		else
		{
			ReportTokenErrorToHandler(tokens.front(), "Couldnt parse this. Starting from " + tokens.front().Content() );
			return false;
		}
	}

	return true;
}

bool SSInterpreter::TryToParseIf(std::vector<SharedExpr> &thecode, std::queue<Token>& tokens_real)
{
	queue<Token> tokens = tokens_real;
	queue<Token> e_tokens;

	std::vector<SharedExpr> else_bodycodeelement;
	bool else_parsed = false;

	if (tokens.size() >= 6)
	{
		Token ifword = tokens.front(); tokens.pop();
		Token parenthopen = tokens.front(); tokens.pop();

		queue<Token> binaryexpr_cond;
		stack<Token> braces;

		if (parenthopen.Content() == "(")
		{
			braces.push(parenthopen);
		}
		else
		{
			//error
			return false;
		}

		while (tokens.size() > 2)
		{
			Token next = tokens.front(); 
			if (next.Content() == "(") braces.push(next);
			else if (next.Content() == ")")
			{
				if (!braces.empty() && braces.top().Content() == "(")
				{
					braces.pop();
				}
				else
				{
					//error mismatching braces
					return false;
				}

				if (braces.empty()) break;
			}
			tokens.pop();
			binaryexpr_cond.push(next);
		}

		SharedExpr astcond = ParseBinaryExpr(binaryexpr_cond);

		Token parenthclose = tokens.front(); tokens.pop();
		Token braceopen = tokens.front(); tokens.pop();

		stack<Token> curlybraces;
		if (braceopen.Content() == "{")
		{
			curlybraces.push(braceopen);
		}

		queue<Token> body;
		while (tokens.size() > 0)
		{
			Token next = tokens.front();

			if (next.Content() == "{") curlybraces.push(next);
			else if (next.Content() == "}")
			{
				if (!curlybraces.empty() && curlybraces.top().Content() == "{")
				{
					curlybraces.pop();
				}
				else
				{
					//error mismatching braces
					return false;
				}

				if (curlybraces.empty()) break;
			}

			tokens.pop();
			body.push(next);
		}

		if (tokens.empty())
		{
			//error, } not found
			return false;
		}
		else
		{

			std::vector<SharedExpr> bodycodeelement;
			bool bodyparsed = ParseCode(bodycodeelement, body);

			Token braceclose = tokens.front(); tokens.pop();

			//else code parsing
			bool elseerror = false;
			e_tokens = tokens;
			if (!e_tokens.empty())
			{
				Token elsetok = e_tokens.front(); e_tokens.pop();
				if (elsetok.Is(TokenType::Identifier, "else"))
				{
					Token e_braceopen = e_tokens.front(); e_tokens.pop();

					stack<Token> e_curlybraces;
					if (braceopen.Content() == "{")
					{
						e_curlybraces.push(e_braceopen);
					}

					queue<Token> e_body;
					while (e_tokens.size() > 0)
					{
						Token next = e_tokens.front();

						if (next.Content() == "{") e_curlybraces.push(next);
						else if (next.Content() == "}")
						{
							if (!e_curlybraces.empty() && e_curlybraces.top().Content() == "{")
							{
								e_curlybraces.pop();
							}
							else
							{
								//error mismatching braces
								//return false;
								elseerror = true;
								break;
							}

							if (e_curlybraces.empty()) break;
						}

						e_tokens.pop();
						e_body.push(next);
					}

					if (e_tokens.empty())
					{
						//error, } not found
						//return false;
						elseerror = true;
					}
					else if (!elseerror)
					{
						Token braceclose = e_tokens.front(); e_tokens.pop();

						else_parsed = ParseCode(else_bodycodeelement, e_body);
					}
				}
			}
			//else code parsing

			if (ifword.Content() == "if" && parenthopen.Content() == "(" && parenthclose.Content() == ")"
				&& braceopen.Content() == "{" && braceclose.Content() == "}" && astcond && bodyparsed)
			{
				SharedIf theif(new ASt_If);
				theif->SetCondition(astcond);
				theif->SetCode(bodycodeelement);
				if (else_parsed) theif->SetElseCode(else_bodycodeelement);

				thecode.push_back(theif);

				if (else_parsed) tokens_real = e_tokens;
				else tokens_real = tokens;

				return true;
			}
		}
	}
	
	return false;
}

bool SSInterpreter::TryToParseFor(std::vector<SharedExpr> &thecode, std::queue<Token>& tokens_real)
{
	queue<Token> tokens = tokens_real;

	vector<SharedExpr> block_first, block_last; // for (block_first; binarycond; block_last)


	if (tokens.size() >= 7)
	{
		Token forword = tokens.front(); tokens.pop();
		Token parenthopen = tokens.front(); tokens.pop();

		if (forword.Content() == "for" && parenthopen.Content() == "(")
		{
			if (!TryToParseAssignment(block_first, tokens, true))
			{
				//error
				return false;
			}
		}
		else
		{
			//error
			return false;
		}

		queue<Token> binaryexpr_cond;
		while (!tokens.empty())
		{
			Token next = tokens.front();
			if (next.Type() == TokenType::Symbol && next.Content() == ";") break;
			tokens.pop();
			binaryexpr_cond.push(next);
		}

		SharedExpr astcond = ParseBinaryExpr(binaryexpr_cond);
		if (!astcond)
		{
			//error
			return false;
		}

		if (tokens.empty())
		{
			//error no ; found
			return false;
		}
		
		Token semicolon = tokens.front(); tokens.pop();

		if (!TryToParseAssignment(block_last, tokens, true))
		{
			//error
			return false;
		}

		if (tokens.size() >= 3) //at least: ){}
		{
			Token parenthclose = tokens.front(); tokens.pop();
			Token braceopen = tokens.front(); tokens.pop();

			stack<Token> curlybraces;
			if (parenthclose.Content() == ")" && braceopen.Content() == "{")
			{
				curlybraces.push(braceopen);

				queue<Token> body;
				while (!tokens.empty())
				{
					Token next = tokens.front(); tokens.pop();

					if (next.Type() == TokenType::Symbol && next.Content() == "{")
					{
						curlybraces.push(next);
					}
					else if (next.Type() == TokenType::Symbol && next.Content() == "}")
					{
						if (curlybraces.top().Content() == "{") curlybraces.pop();
						else
						{
							//error
							return false;
						}
					}

					if (curlybraces.empty())
					{
						std::vector<SharedExpr> bodycodeelement;
						bool bodyparsed = ParseCode(bodycodeelement, body);

						if (bodyparsed)
						{
							SharedFor thefor(new AST_For(
								std::static_pointer_cast<AST_Assignment>(block_first[0]),
								std::static_pointer_cast<AST_BinaryExpr>(astcond),
								std::static_pointer_cast<AST_Assignment>(block_last[0]),
								bodycodeelement));

							thecode.push_back(thefor);
							tokens_real = tokens;
							return true;
						}
						else
						{
							//error
							return false;
						}
					}
					else
					{
						body.push(next);
					}
				}

				//error: didnt hit close brace }
				return false;
			}
			else
			{
				//error
				return false;
			}
		}
		else
		{
			//error
			return false;
		}
	}
	else
	{
		//error
		return false;
	}
}

bool SSInterpreter::TryToParseAssignment(std::vector<SharedExpr> &thecode, std::queue<Token>& tokens_real, bool finalsemicolon)
{
	queue<Token> tokens = tokens_real;

	if (tokens.size() >= 3)
	{
		Token var = tokens.front(); tokens.pop();
		Token type;

		if (IsTypename(var.Content()))
		{
			type = var;
			var = tokens.front(); tokens.pop();
		}

		Token equals_or_semicolon = tokens.front(); tokens.pop();
		
		if (equals_or_semicolon.Content() == ";")
		{
			SharedAssignment ass(new AST_Assignment(*this));
			SharedVar refdvar(new AST_Variable(type.Content(), var.Content()));
			ass->SetVar(refdvar);
			ass->SetVarValue(nullptr);
			tokens_real = tokens;
			thecode.push_back(ass);
			return true;
		}

		queue<Token> binaryexpr;
		while (tokens.size() > 0)
		{
			Token next = tokens.front(); 
			if (next.Content() == ";") break;
			tokens.pop();
			binaryexpr.push(next);
		}

		Token semicolon;
		if (finalsemicolon && !tokens.empty())
		{
			semicolon = tokens.front(); tokens.pop();
		}

		if (var.Type() == TokenType::Identifier && equals_or_semicolon.Content() == "=" && (!finalsemicolon || semicolon.Content() == ";")
			&& binaryexpr.size() > 0)
		{
			SharedExpr bin = ParseBinaryExpr(binaryexpr);
			if (bin)
			{
				SharedAssignment ass(new AST_Assignment(*this));
				SharedVar refdvar(new AST_Variable(type.Content(), var.Content()));
				ass->SetVar(refdvar);
				ass->SetVarValue(bin);
				tokens_real = tokens;
				thecode.push_back(ass);
				return true;
			}
		}
	}
	
	return false;
}

bool SSInterpreter::TryToParseMemberAssignment(std::vector<SharedExpr>& thecode, std::queue<Token>& tokens_real)
{
	queue<Token> tokens = tokens_real;

	queue<Token> memberaccess;

	if (tokens.size() < 7) //a[b]=c;
	{
		//error
		return false;
	}

	Token objname = tokens.front(); tokens.pop();
	Token brace = tokens.front(); tokens.pop();

	if (objname.Type() == TokenType::Identifier && brace.Is(TokenType::Symbol, "["))
	{
		tokens = tokens_real;

		while (!tokens.empty())
		{
			Token next = tokens.front();
			if (next.Is(TokenType::Symbol, "=")) break;
			else memberaccess.push(next);
			tokens.pop();
		}

		if (tokens.empty())
		{
			//error havent found =
			return false;
		}
		else
		{
			tokens.pop(); //pop =

			SharedExpr leftoperand = ParseNextOperand(memberaccess);
			if (leftoperand)
			{
				queue<Token> rightsideexpr;
				while (tokens.size() > 0)
				{
					Token next = tokens.front();
					if (next.Is(TokenType::Symbol,";")) break;
					tokens.pop();
					rightsideexpr.push(next);
				}

				if (tokens.empty())
				{
					//error havent found ;
					return false;
				}
				else
				{
					tokens.pop();

					SharedExpr rightsidebin = ParseBinaryExpr(rightsideexpr);
					if (rightsidebin)
					{
						SharedAssignment ass(new AST_Assignment(*this));
						ass->SetVar(leftoperand);
						ass->SetVarValue(rightsidebin);
						tokens_real = tokens;
						thecode.push_back(ass);
						return true;
					}
					else
					{
						//error
						return false;
					}
				}
			}
			else
			{
				//error
				return false;
			}
		}
	}
	else
	{
		//error
		return false;
	}
}

bool SSInterpreter::TryToParseReturn(std::vector<SharedExpr> &thecode, std::queue<Token>& real_tokens)
{
	std::queue<Token> tokens = real_tokens;

	if (tokens.size() >= 2)
	{
		Token ret = tokens.front(); tokens.pop();

		if (ret.Content() == "return")
		{
			std::queue<Token> binexpr;

			while (!tokens.empty())
			{
				Token next = tokens.front(); 
				if (next.Type() == TokenType::Symbol && next.Content() == ";")
				{
					break;
				}
				tokens.pop();

				binexpr.push(next);
			}

			if (!tokens.empty())
			{
				tokens.pop(); //pop last ";"
				SharedExpr retval = ParseBinaryExpr(binexpr);

				if (!retval && !binexpr.empty())
				{
					//error in parsing binary
					return false;
				}
				
				real_tokens = tokens;
				thecode.push_back(SharedReturn(new AST_Return(retval)));
				return true;
			}
			else
			{
				//error
				return false;
			}
		}
	}

	return false;
}

bool SSInterpreter::TryToParseFuncCall(std::vector<SharedExpr>& thecode, std::queue<Token>& tokens_real, bool finalsemicolon)
{
	queue<Token> tokens = tokens_real;

	auto expr = ParseNextOperand(tokens);

	if (expr && dynamic_cast<AST_FunctionCalling*>(expr.get()) && !tokens.empty())
	{
		Token next;
		
		if (finalsemicolon)
		{
			next = tokens.front(); tokens.pop();
		}

		if (!finalsemicolon || next.Content() == ";")
		{
			tokens_real = tokens;
			thecode.push_back(expr);
			return true;
		}
	}

	return false;
}

bool SSInterpreter::ParseCommaSeparatedOperands(std::vector<SharedExpr> &operands, std::queue<Token>& tokens)
{
	if (tokens.empty()) return true;

	std::queue<Token> singleoperandtokens;
	std::stack<Token> braces;

	while (true)
	{
		if (!tokens.empty())
		{
			if (tokens.front().Is(TokenType::Symbol, "(")) braces.push(tokens.front());
			else if (tokens.front().Is(TokenType::Symbol, ")"))
			{
				if (braces.top().Is(TokenType::Symbol, "("))
				{
					braces.pop();
				}
				else
				{
					//error mismatched parentheses
					return false;
				}
			}
		}

		if (tokens.empty() || (braces.empty() && tokens.front().Is(TokenType::Symbol,",")))
		{
			if (!tokens.empty()) tokens.pop(); //pop comma

			if (singleoperandtokens.empty())
			{
				//error, empty operand
				return false;
			}
			else
			{
				SharedExpr op = ParseBinaryExpr(singleoperandtokens);
				if (op)
				{
					operands.push_back(op);
					while (!singleoperandtokens.empty()) singleoperandtokens.pop();

					if (tokens.empty()) return true;
				}
				else
				{
					//error, fail to parse
					return false;
				}
			}
		}
		else
		{
			Token next = tokens.front(); tokens.pop();
			singleoperandtokens.push(next);
		}
	}

	return true;
}

void SSInterpreter::RegisterTypename(std::string thetypename)
{
	typenames.emplace(thetypename);
}

bool SSInterpreter::IsTypename(std::string s)
{
	return typenames.find(s) != typenames.end();
}

SharedExpr SSInterpreter::ParseBinaryExpr(std::queue<Token>& tokens)
{
	std::queue<SharedExpr> output;
	std::stack<Token> opstack;

	while (!tokens.empty())
	{
		Token t = tokens.front(); 

		if (IsOperator(t))
		{
			tokens.pop();

			while (!opstack.empty() && GetPrecedence(opstack.top()) > GetPrecedence(t))
			{
				Token op_from_stacktop = opstack.top(); opstack.pop();
				output.push(SharedBinExpr(new AST_BinaryExpr(op_from_stacktop)));
			}

			opstack.push(t);
		}
		else if (t.Type() == TokenType::Symbol && t.Content() == "(")
		{
			tokens.pop();
			opstack.push(t);
		}
		else if (t.Type() == TokenType::Symbol && t.Content() == ")")
		{
			tokens.pop();
			while (!opstack.empty() && opstack.top().Type() == TokenType::Symbol && opstack.top().Content() != "(")
			{
				Token op_from_stacktop = opstack.top(); opstack.pop();
				output.push(SharedBinExpr(new AST_BinaryExpr(op_from_stacktop)));
			}

			if (opstack.empty())
			{
				//error : no left brcket
				return nullptr;
			}
			else opstack.pop();
		}
		else
		{
			SharedExpr operand = ParseNextOperand(tokens);
			if (operand)
			{
				output.push(operand);
			}
			else
			{
				//error couldnt parse operand
				return nullptr;
			}
		}
	}

	std::stack<SharedExpr> finalout;

	while (!opstack.empty())
	{
		output.push(SharedBinExpr(new AST_BinaryExpr(opstack.top()))); 
		opstack.pop();
	}

	while (!output.empty())
	{
		SharedExpr next = output.front(); output.pop();

		AST_BinaryExpr* binexpr = dynamic_cast<AST_BinaryExpr*>(next.get());
		if (binexpr)
		{
			if (finalout.size() >= 2)
			{
				SharedExpr rightoperand = finalout.top(); finalout.pop();
				SharedExpr leftoperand = finalout.top(); finalout.pop();

				binexpr->SetOperands(leftoperand, rightoperand);

				finalout.push(next);
			}
			else
			{
				//error mismatched operands
				return nullptr;
			}
		}
		else finalout.push(next);
	}

	if (finalout.size() == 1) return finalout.top();
	else
	{
		//error, should only have one root of expression
		return nullptr;
	}
}

SharedExpr SSInterpreter::ParseNextOperand(std::queue<Token>& tokens)
{
	if (tokens.empty()) return nullptr;

	Token first = tokens.front(); tokens.pop();

	if (first.Type() == TokenType::Number)
	{
		return SharedNumber(new AST_Number(first.Content()));
	}
	else if (first.Type() == TokenType::StringLiteral)
	{
		return SharedStringLiteral(new AST_StringLiteral(first.Content()));
	}
	else if (first.Type() == TokenType::Identifier)
	{
		Token next_peek;
		
		if (!tokens.empty()) next_peek = tokens.front();

		if (next_peek.Type() == TokenType::Symbol && next_peek.Content() == "." && tokens.size() >= 3) //method call
		{
			tokens.pop();

			Token funcname = tokens.front(); tokens.pop();

			Token brace = tokens.front(); tokens.pop();

			if (funcname.Type() == TokenType::Identifier && brace.Content() == "(")
			{
				stack<Token> braces;
				braces.push(brace);

				std::queue<Token> arguments;

				while (!tokens.empty())
				{
					Token next = tokens.front(); tokens.pop();
					if (next.Content() == "(") braces.push(next);
					else if (next.Content() == ")")
					{
						if (!braces.empty() && braces.top().Content() == "(")
						{
							braces.pop();
							if (braces.empty()) break; //parsed last ')'
						}
						else
						{
							//error mismatched parentheses
							return nullptr;
						}
					}
					
					arguments.push(next);
				}

				std::vector<SharedExpr> parsedarguments;
				bool args_ok = ParseCommaSeparatedOperands(parsedarguments, arguments);

				if (!args_ok)
				{
					//error
					return nullptr;
				}
				else return SharedFuncCalling(new AST_FunctionCalling(first.Content(), funcname.Content(), parsedarguments));
			}
			else
			{
				//error
				return nullptr;
			}
		}
		else if (next_peek.Is(TokenType::Symbol,"[") && tokens.size() >= 3)
		{
			stack<Token> braces;
			braces.push(next_peek);
			tokens.pop();

			queue<Token> binexpr;

			while (!tokens.empty())
			{
				Token bintok = tokens.front();

				if (bintok.Is(TokenType::Symbol, "[")) braces.push(bintok);
				else if (bintok.Is(TokenType::Symbol, "]"))
				{
					if (braces.top().Is(TokenType::Symbol, "["))
					{
						braces.pop();
						if (braces.empty()) break;
					}
					else
					{
						//error mismatched []
						return nullptr;
					}
				}
				else binexpr.push(bintok);

				tokens.pop();
			}

			if (tokens.empty())
			{
				//error, havent found ]
				return nullptr;
			}
			else
			{
				tokens.pop(); //pop ]

				SharedExpr memberexpr = ParseBinaryExpr(binexpr);

				if (memberexpr)
				{
					return SharedMemberAccess(new AST_MemberAccess(first.Content(), memberexpr));
				}
				else
				{
					//error parsing between []
					return nullptr;
				}
			}
		}
		else //variable
		{
			return SharedVar(new AST_Variable("", first.Content()));
		}
	}

	return nullptr;
}

void SSInterpreter::RegisterCustomDataTypesFromCode(std::vector<Token>& tokens)
{
	for (int i = 0; i < int(tokens.size()) - 2; ++i)
	{
		if (
			tokens[i].Is(TokenType::Identifier, "table")
			&& tokens[i+1].Type() == TokenType::Identifier
			&& (tokens[i+2].Is(TokenType::Symbol, "{") || tokens[i + 2].Is(TokenType::Symbol, ":"))
			)
		{
			RegisterTypename(tokens[i + 1].Content());
		}
	}
}

Table * SSInterpreter::InstantiateTable(std::string type_name)
{
	return InstantiateTableWithParsedArgs(type_name, {});
}

Table * SSInterpreter::InstantiateTableWithParsedArgs(std::string type_name, std::vector<Variable> args)
{
	auto found = tabletemplates.find(type_name);
	if (found != tabletemplates.end())
	{
		Table *t = new Table(type_name);
		found->second.InstantiateTable(t, tabletemplates);
		return t;
	}
	return nullptr;
}

void SSInterpreter::RegisterDefaultTypenames()
{
	RegisterTypename("int");
	RegisterTypename("float");
	RegisterTypename("string");
	RegisterTypename("table");
	RegisterTypename("void");
	RegisterTypename("ss");
}

void SSInterpreter::RegisterOperators()
{
	operators["||"] = 60;
	operators["&&"] = 70;

	operators["=="] = 75;
	operators["!="] = 75;

	operators[">"] = 80;
	operators["<"] = 80;
	operators[">="] = 80;
	operators["<="] = 80;
	operators["-"] = 90;
	operators["+"] = 90;
	operators["*"] = 100;
	operators["/"] = 100;
}

bool SSInterpreter::IsOperator(Token & t)
{
	return t.Type() == TokenType::Symbol && operators.find(t.Content()) != operators.end();
}

int SSInterpreter::GetPrecedence(Token & t)
{
	auto found = operators.find(t.Content());
	return found == operators.end() ? -1 : found->second;
}

std::string SSInterpreter::GetFileNameFromPath(std::string path)
{
	auto lastslash = path.find_last_of("\\/");
	if (lastslash == string::npos) return path;
	else return path.substr(lastslash+1);
}

void SSInterpreter::SetDefaultApiCollection()
{
	apicollection.AddMethod("ss", "appendtofile", SharedFunctor(new AddLineToFileFunctor()), *this);
	apicollection.AddMethod("ss", "print", SharedFunctor(new ConsolePrintFunctor()), *this);
	apicollection.AddMethod("ss", "copy", SharedFunctor(new CopyFunctor()), *this);
	apicollection.AddMethod("*", "hasvars", SharedFunctor(new HasVarFunctor()), *this);
	apicollection.AddMethod("ss", "readfile", SharedFunctor(new ReadFileFunctor()), *this);
	apicollection.AddMethod("ss", "readline", SharedFunctor(new ReadLineFunctor()), *this);
	apicollection.AddMethod("ss", "split", SharedFunctor(new SplitToWordsFunctor()), *this);
	apicollection.AddMethod("ss", "concat", SharedFunctor(new StringConcat()), *this);

	apicollection.AddMethod("*", "size", SharedFunctor(new GetSizeFunctor()), *this);
	apicollection.AddMethod("*", "delete", SharedFunctor(new DeleteVarFunctor()), *this);
	apicollection.AddMethod("ss", "tostring", SharedFunctor(new ToStringFunctor()), *this);
	apicollection.AddMethod("ss", "tofloat", SharedFunctor(new ToFloatFunctor()), *this);
	apicollection.AddMethod("ss", "toint", SharedFunctor(new ToIntFunctor()), *this);

	apicollection.AddMethod("*", "getvars", SharedFunctor(new GetKeysFunctor()), *this);
}

void SSInterpreter::ReportTokenErrorToHandler(Token t, std::string message)
{
	errhandler.OnMsg("Line " + to_string(t.GetLineNum()) + ":" + message);
}
