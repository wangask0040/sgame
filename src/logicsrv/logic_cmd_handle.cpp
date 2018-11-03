#include "logic_cmd_handle.h"
#include "protocol/msg_cmd.pb.h"

using namespace protocol;

CLogin::CLogin()
{
}

CLogin::~CLogin()
{
}

int CLogin::Enter(const CMD_HANDLE_REQ& req, CMD_HANDLE_REPLY& reply)
{
	return msgresult_start;
}