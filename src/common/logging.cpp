#include "logging.h"
#include <assert.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

bool CLog::init(const char* name, const LOG_CONFIG& logconf, int idx)
{
	google::InitGoogleLogging(name);
	if (nullptr == opendir(logconf.dir.c_str()))
	{
		auto ret = mkdir(logconf.dir.c_str(), S_IRWXU);
		assert(ret == 0);
		if (ret != 0)
			return false;
	}

	std::stringstream ss;
	ss << logconf.dir << "/" << name << LOGDIRSPLIT << idx;
	if (nullptr == opendir(ss.str().c_str()))
	{
		auto ret = mkdir(ss.str().c_str(), S_IRWXU);
		assert(ret == 0);
		if (ret != 0)
			return false;
	}
	
	FLAGS_log_dir = ss.str().c_str(); //log的目录
	FLAGS_logbufsecs = 0;//log在缓存中缓存的秒数，0表示不缓存，写就落地磁盘
	FLAGS_stderrthreshold = logconf.level;//log级别大于该值的才同时输出到控制台
	FLAGS_colorlogtostderr = true;//log有颜色区分
	FLAGS_stop_logging_if_full_disk = true;//磁盘写满了就不写了
	return true;
}

void CLog::uninit()
{
	google::ShutdownGoogleLogging();
}


bool InitLog(const char* module_name, const LOG_CONFIG& logConfig, int idx)
{
	CLog l;
	return l.init(module_name, logConfig, idx);
}