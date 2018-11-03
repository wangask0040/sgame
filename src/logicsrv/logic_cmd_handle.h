#pragma once
#include "protocol/cmd_handle.h"

class CLogin
	: public ICmdHandleUnit
{
public:
	CLogin();
	~CLogin();
	int Enter(const CMD_HANDLE_REQ& req, CMD_HANDLE_REPLY& reply) override;
private:

};

