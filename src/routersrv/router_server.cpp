#include "router_server.h"
#include "common/zmqex_poll_wrapper.h"
#include "config/router_config.h"
#include "common/logging.h"

CRouterSrv::CRouterSrv()
{
}

CRouterSrv::~CRouterSrv()
{}

void CRouterSrv::Start()
{
	while (true)
	{
		auto ret = CZmqPollWrapperSingleton::Instance()->Wait(-1);
		if (ret > 0)
			CZmqPollWrapperSingleton::Instance()->Proc(ret);
		else
			NLOGERROR << ret;
	}
}

bool CRouterSrv::Init(int srv_type, int srv_idx)
{
	_srv_type = srv_type;
	_srv_idx = srv_idx;
	
	auto ptr = CRouterConfigSgt::Instance()->Get(srv_idx);
	if (CMsgMqWrapperSingleton::Instance()->Init(*ptr))
		return CZmqPollWrapperSingleton::Instance()->Init();
	else
		return false;
}
