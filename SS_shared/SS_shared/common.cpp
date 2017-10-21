#include "stdafx.h"
#include "common.h"

std::vector<std::string> split(const std::string & text, char sep) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

bool isnumber(std::string & str)
{
	for (int i = 0; i < str.size(); ++i)
	{ 
		if (!isdigit(str[i]) && !(i == 0 && str[i] == '-')) return false;
	}
	return true;
}
