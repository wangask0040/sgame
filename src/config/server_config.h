#pragma once

#include "loadxml.h"
#include "srvconfig.h"

class CServerConfig
	: public ILoadXml
{
public:
	CServerConfig();
	~CServerConfig();
	bool Init(const char* file, const std::string& name);
	bool Read(tinyxml2::XMLElement* root) override;
	void Clear() override;
	const SERVER_CONFIG* GetInfoPtr(const char* name) const;//通过服务器的进程名字取出对应的配置
	const LOG_CONFIG& LogConfig() const { return _log_config; }

	bool LoadAllServerCfg() const;
	bool LoadAllDataCfg() const;

	std::string GenSeverCfgFullPath(const char* filename) const;

	static bool IsSrvType(int v);
	static bool IsDbType(int v);

private:
	MAP_SERVER_CONFIG _config;
	std::string _config_root_dir;
	LOG_CONFIG _log_config;
};

using CServerConfigSgt = Singleton<CServerConfig>;
