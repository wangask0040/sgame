#pragma once
#include "protocol/cmd_handle.h"

class CGameReload
	: public ICmdHandleUnit
{
public:
	CGameReload() {}
	~CGameReload() {}
	int Enter(const CMD_HANDLE_REQ& req, CMD_HANDLE_REPLY& reply) override;
private:

};