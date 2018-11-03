#pragma once
#include "protocol/cmd_handle.h"

class CRestartSrv
	: public ICmdHandleUnit
{
public:
	CRestartSrv() {}
	~CRestartSrv() {}
	int Enter(const CMD_HANDLE_REQ& req, CMD_HANDLE_REPLY& reply) override;
private:
};
