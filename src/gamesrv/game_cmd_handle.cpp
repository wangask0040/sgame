#include "game_cmd_handle.h"
#include "config/server_config.h"
#include "common/logging.h"

int CGameReload::Enter(const CMD_HANDLE_REQ& req, CMD_HANDLE_REPLY& reply)
{
	auto ret = CServerConfigSgt::Instance()->LoadAllDataCfg();
	NLOGDEBUG << "reload ret:" << ret;
	return 1;
}