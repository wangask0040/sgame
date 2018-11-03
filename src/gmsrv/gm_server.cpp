#include "gm_server.h"
#include "protocol/cmd_handle.h"
#include "protocol/msg_cmd.pb.h"
#include "gm_cmd_handle.h"
#include "common/logging.h"
#include "config/router_config.h"
#include "common/zmqex_connect_wrapper.h"

using namespace protocol;

bool CGmSrv::Init(int srvtype, int srvidx)
{
	CCmdHanleMgrSgt::Instance()->Add(msgcmd_gm_reload, reinterpret_cast<ICmdHandleUnit*>(new CRestartSrv()));


	_srv_type = srvtype;
	_srv_idx = srvidx;

	auto routerid = ROUTER_INFO::GenRouterId(srvtype, srvidx);
	VEC_ROUTER_INFO vec;
	CRouterConfigSgt::Instance()->GetRouterConnectVec(vec, routerid);
	auto ret = CZmqRouterClientWrapperSgt::Instance()->Init(vec);
	return ret == 0;
}

void CGmSrv::HandleHttpReq(evhttp_request* req)
{
	CMD_HANDLE_REQ cmdreq;
	CMD_HANDLE_REPLY cmdreply;

	GetUrlPath(req);

	auto query = GetUrlQuery(req);

	MAP_URL_PARAM mp;
	QuerySplit(query, mp);
	auto cmd = atoi(mp["cmd"].c_str());
	cmdreq.info.mutable_head()->set_cmd(cmd);

	CCmdHanleMgrSgt::Instance()->Handle(cmdreq, cmdreply);

	IHttpServer::ResponseBodyData(req, cmdreply.data);
}
