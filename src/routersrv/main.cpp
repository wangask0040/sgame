#include "router_server.h"
#include <iostream>
#include <vector>
#include "common/stringex.h"
#include "common/logging.h"
#include "config/server_config.h"
#include "common/struct_def.h"

int main(int argc, char** argv)
{
	if (argc < 1)
	{
		std::cout << "etc:program configfile" << std::endl;
		return 1;
	}

	std::vector<std::string> vec;
	split(argv[0], DIRSPLIT, vec);
	auto name = *vec.rbegin();

	if (!CServerConfigSgt::Instance()->Init(argv[1], name))
		return 1;

	auto& log = CServerConfigSgt::Instance()->LogConfig();
	auto ptr = CServerConfigSgt::Instance()->GetInfoPtr(name.c_str());
	InitLog(name.c_str(), log, ptr->idx);

	CRouterSrv srv;
	if (srv.Init(static_cast<int>(SRV_TYPE::rt_routersrv), ptr->idx))
	{
		NLOGINFO << name << " start.";
		srv.Start();
	}
	return 1;
}