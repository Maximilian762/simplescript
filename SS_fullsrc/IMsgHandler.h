#pragma once
#include <string>

class IMsgHandler
{
public:
	IMsgHandler();
	virtual ~IMsgHandler();

	virtual void OnMsg(std::string msg);
};

