#include "logic_server.h"
#include "protocol/msg_info.pb.h"
#include "protocol/msg_cmd.pb.h"
#include "protocol/msg_assist.h"
#include "protocol/cmd_handle.h"
#include "logic_cmd_handle.h"

using namespace protocol;

CLogicSrv::CLogicSrv()
{
}


CLogicSrv::~CLogicSrv()
{
}

void CLogicSrv::HandleHttpReq(evhttp_request* req)
{
	std::string data;
	IHttpServer::GetBodyData(req, data);

	CMD_HANDLE_REQ cmdreq;
	CMD_HANDLE_REPLY cmdreply;

	bool ret = false;
	CMsgAssist::decode(data.c_str(), data.size(), cmdreq.info, ret);
	if (ret)
	{
		CCmdHanleMgrSgt::Instance()->Handle(cmdreq, cmdreply);
	}
	else
	{
		protocol::msg_info tmp;
		tmp.mutable_head()->set_result(msgresult_parse_err);
		tmp.SerializeToString(&cmdreply.data);
	}

	IHttpServer::ResponseBodyData(req, cmdreply.data);
}

bool CLogicSrv::Init() const
{
	CCmdHanleMgrSgt::Instance()->Add(msgcmd_logic_login, reinterpret_cast<ICmdHandleUnit*>(new CLogin()));

	return true;
}

