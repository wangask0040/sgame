#pragma once
#include "logconfig.h"
#include <unordered_map>
#include <string>
#include "common/stringex.h"
#include <sstream>

struct SERVER_CONFIG
{
	LOG_CONFIG logcfg;
	
	//�������еķ����������������ֶ�
	std::string ip;
	int port;

	int idx;//ͬһ�����������̻����������������ֶ�������
	
	const std::string Addr() const
	{
		std::stringstream ss;
		ss << ip << ":" << port;
		return ss.str();
	}
};

using MAP_SERVER_CONFIG = std::unordered_map<std::string, SERVER_CONFIG>;

