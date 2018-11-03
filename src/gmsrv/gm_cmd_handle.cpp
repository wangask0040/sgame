#include "gm_cmd_handle.h"
#include "common/zmqex_connect_wrapper.h"
#include "protocol/msg_cmd.pb.h"
#include "gm_server.h"
#include "config/router_config.h"
#include "common/logging.h"
#include "protocol/router_send_assist.h"

using namespace protocol;

int CRestartSrv::Enter(const CMD_HANDLE_REQ& req, CMD_HANDLE_REPLY& reply)
{
	unsigned int srv_type = CGmSrvSgt::Instance()->SrvType();
	unsigned int srv_idx = CGmSrvSgt::Instance()->SrvIdx();

	auto mpinfo = CRouterConfigSgt::Instance()->Get(0);
	for (auto& it : *mpinfo)
	{
		if (it.first != srv_type)
		{
			auto& vec = it.second;
			for (size_t i = 0; i < vec.size(); ++i)
			{
				CRouterSend::GmToGame(srv_idx, i, msgcmd_gm_reload, 0);
			}
		}
	}
	return 1;
}