#include "game_server.h"
#include "common/struct_def.h"
#include "config/router_config.h"
#include "common/zmqex_connect_wrapper.h"
#include "common/logging.h"
#include <assert.h>
#include "protocol/msg_cmd.pb.h"
#include "protocol/cmd_handle.h"
#include "game_cmd_handle.h"


using namespace protocol;

CGameSrv::CGameSrv()
{
}


CGameSrv::~CGameSrv()
{
}

void CGameSrv::Start()
{
	while (true)
	{
		auto ret = CZmqRouterClientWrapperSgt::Instance()->Wait(-1);
		if (ret > 0)
		{
			int cur_num = 0;
			while (cur_num < ret)
			{
				MSG_INNER_INFO frame;
				if (CZmqRouterClientWrapperSgt::Instance()->RecvFrame(frame))
				{
					cur_num++;

					CMD_HANDLE_REQ cmdreq;
					CMD_HANDLE_REPLY cmdreply;
					cmdreq.info.ParseFromString(frame.data);
					
					CCmdHanleMgrSgt::Instance()->Handle(cmdreq, cmdreply);
				}
				else
				{
					assert(false);
					NLOGERROR << "recv err.";
				}
			}
		}

	}
}

bool CGameSrv::Init(int srvtype, int srvidx)
{
	CCmdHanleMgrSgt::Instance()->Add(msgcmd_gm_reload, reinterpret_cast<ICmdHandleUnit*>(new CGameReload()));

	_srv_type = srvtype;
	_srv_idx = srvidx;

	auto routerid = ROUTER_INFO::GenRouterId(srvtype, srvidx);
	VEC_ROUTER_INFO vec;
	CRouterConfigSgt::Instance()->GetRouterConnectVec(vec, routerid);
	auto ret = CZmqRouterClientWrapperSgt::Instance()->Init(vec);
	return ret == 0;
}
