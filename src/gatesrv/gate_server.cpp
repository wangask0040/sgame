#include "gate_server.h"
#include "common/struct_def.h"
#include "config/router_config.h"
#include "common/zmqex_connect_wrapper.h"
#include "protocol/msg_assist.h"
#include "protocol/msg_cmd.pb.h"

using namespace protocol;

CGateSrv::CGateSrv()
{
}


CGateSrv::~CGateSrv()
{
}

void CGateSrv::OnWSRead(const std::string& data, bufferevent* bev)
{
	auto fd = bufferevent_getfd(bev);
	
	bool ret = false;
	protocol::msg_info info;
	CMsgAssist::decode(data.c_str(), data.size(), info, ret);
	if (ret)
	{
		auto cmd = info.head().cmd();
		if (cmd > msgcmd_game_start && cmd < msgcmd_game_end)
		{

		}
	}
}

bool CGateSrv::Init(int srvtype, int srvidx)
{
	_srv_type = srvtype;
	_srv_idx = srvidx;

	auto routerid = ROUTER_INFO::GenRouterId(srvtype, srvidx);
	VEC_ROUTER_INFO vec;
	CRouterConfigSgt::Instance()->GetRouterConnectVec(vec, routerid);
	auto ret = CZmqRouterClientWrapperSgt::Instance()->Init(vec);
	return ret == 0;
}
