#pragma once
#include <string>

enum class TokenType
{
	Identifier,
	Number,
	StringLiteral,
	Symbol
};

class Token
{
public:
	Token(std::string _content = "");
	~Token();

	TokenType Type();
	std::string Content();

	bool Is(TokenType t, std::string cmpcontent);

	void SetLine(int _num);
	int GetLineNum();

private:
	TokenType type = TokenType::Symbol;
	std::string content;
	int linenumber = -1;
};

