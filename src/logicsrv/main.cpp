#include <iostream>
#include "config/server_config.h"
#include "common/logging.h"
#include "common/stringex.h"
#include "logic_server.h"

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

	CLogicSrv srv;
	srv.Init();
	NLOGINFO << name << " start.";
	srv.start(ptr->ip, ptr->port);
	return 1;
}
