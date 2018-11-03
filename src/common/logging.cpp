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
	
	FLAGS_log_dir = ss.str().c_str(); //log��Ŀ¼
	FLAGS_logbufsecs = 0;//log�ڻ����л����������0��ʾ�����棬д����ش���
	FLAGS_stderrthreshold = logconf.level;//log������ڸ�ֵ�Ĳ�ͬʱ���������̨
	FLAGS_colorlogtostderr = true;//log����ɫ����
	FLAGS_stop_logging_if_full_disk = true;//����д���˾Ͳ�д��
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