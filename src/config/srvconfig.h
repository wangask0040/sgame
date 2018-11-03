#pragma once
#include "logconfig.h"
#include <unordered_map>
#include <string>
#include "common/stringex.h"
#include <sstream>

struct SERVER_CONFIG
{
	LOG_CONFIG logcfg;
	
	//不是所有的服务器都有这两个字段
	std::string ip;
	int port;

	int idx;//同一个服务器进程会启动多个，用这个字段来区分
	
	const std::string Addr() const
	{
		std::stringstream ss;
		ss << ip << ":" << port;
		return ss.str();
	}
};

using MAP_SERVER_CONFIG = std::unordered_map<std::string, SERVER_CONFIG>;

