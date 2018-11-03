#pragma once

#include <glog/logging.h>
#include <glog/log_severity.h>
#include "config/logconfig.h"

#define LOGSPLIT '|'
#define LOGDIRSPLIT '-'

static const std::string DIRSPLIT = "/";
static const std::string URL_PARAM_SPLIT = "&";
static const std::string URL_PARAM_VALUE_SPLIT = "=";

class CLog
{
public:
	CLog() {}
	~CLog() {}

	bool init(const char* name, const LOG_CONFIG& logconf, int idx);
	void uninit();
private:

};

#define NLOGINFO LOG(INFO)<<__FUNCTION__<<LOGSPLIT
#define NLOGWARN LOG(WARNING)<<__FUNCTION__<<LOGSPLIT
#define NLOGERROR LOG(ERROR)<<__FUNCTION__<<LOGSPLIT
#define NLOGDEBUG DLOG(INFO)<<__FUNCTION__<<LOGSPLIT
#define NLOGWARNIF(x) LOG_IF(WARNING, x)<<__FUNCTION__<<LOGSPLIT

bool InitLog(const char* module_name, const LOG_CONFIG& logConfig, int idx);
