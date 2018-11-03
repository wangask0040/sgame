#pragma once
#include <string>
#include "common/tinyxml2.h"

struct LOG_CONFIG
{
	std::string dir;//log目录
	int level;//输出到控制台的log级别
};