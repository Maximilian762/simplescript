#include "stdafx.h"
#include "Token.h"
#include <ctype.h>

Token::Token(std::string _content)
{
	content = _content;

	if (content.empty()) return;

	if (isalpha(content[0]))
	{
		type = TokenType::Identifier;
	}
	else if (isdigit(content[0])) type = TokenType::Number;
	else if (content.length() >= 2 && content.front() == '"' && content.back() == '"')
	{
		content = content.substr(1, content.length() - 2);
		type = TokenType::StringLiteral;
	}
	else type = TokenType::Symbol;
}

Token::~Token()
{
}

TokenType Token::Type()
{
	return type;
}

std::string Token::Content()
{
	return content;
}

bool Token::Is(TokenType t, std::string cmpcontent)
{
	return type == t && content == cmpcontent;
}

void Token::SetLine(int _num)
{
	linenumber = _num;
}

int Token::GetLineNum()
{
	return linenumber;
}
