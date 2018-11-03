#include "server_config.h"
#include <assert.h>
#include "router_config.h"
#include "dbconfig.h"

CServerConfig::CServerConfig()
{
}

CServerConfig::~CServerConfig()
{
}

bool CServerConfig::Read(tinyxml2::XMLElement* root)
{
	_config_root_dir = root->FirstChildElement("config_root_dir")->GetText();

	//先处理log配置
	auto node = root->FirstChildElement("log");
	_log_config.level = node->UnsignedAttribute("level");
	std::stringstream ss;
	ss << _config_root_dir << "/../log";
	_log_config.dir = ss.str();

	//在读取所有服务器配置
	node = root->FirstChildElement("server");
	for (auto item = node->FirstChildElement(); item != nullptr; item = item->NextSiblingElement())
	{
		auto name = item->Attribute("name");
		SERVER_CONFIG sc;
		sc.idx = item->UnsignedAttribute("idx");

		if (item->Attribute("ip") != nullptr)
		{
			sc.ip = item->Attribute("ip");
			sc.port = item->IntAttribute("port");
		}

		auto it =  _config.emplace(std::make_pair(name, sc));
		assert(it.second);
	}

	return _config.size() > 0;
}

void CServerConfig::Clear()
{
	_config.clear();
}

const SERVER_CONFIG* CServerConfig::GetInfoPtr(const char* name) const
{
	auto it = _config.find(name);
	if (it != _config.end())
		return &it->second;
	else
		return nullptr;
}

std::string CServerConfig::GenSeverCfgFullPath(const char* filename) const
{
	std::stringstream ss;
	ss << _config_root_dir << "/server/" << filename << ".xml";
	return ss.str();
}

bool CServerConfig::LoadAllServerCfg() const
{
	auto ret = CRouterConfigSgt::Instance()->Load(GenSeverCfgFullPath("routerconfig"));
	if (!ret) return false;

	ret = CDbConfigSgt::Instance()->Load(GenSeverCfgFullPath("dbconfig"));
	if (!ret) return false;

	return true;
}

bool CServerConfig::LoadAllDataCfg() const
{
	return true;
}

bool CServerConfig::Init(const char* file, const std::string& name)
{
	auto ret = Load(file);
	ret &= LoadAllServerCfg();
	ret &= LoadAllDataCfg();

	return ret;
}

bool CServerConfig::IsSrvType(int v)
{
	SRV_TYPE c = static_cast<SRV_TYPE>(v);
	switch (c)
	{
	case SRV_TYPE::rt_gatesrv:
	case SRV_TYPE::rt_gamesrv:
	case SRV_TYPE::rt_gmsrv:
	case SRV_TYPE::rt_routersrv:
		return true;
		break;
	default:
		break;
	}
	return false;
}


bool CServerConfig::IsDbType(int v)
{
	DB_TYPE c = static_cast<DB_TYPE>(v);
	switch (c)
	{
	case DB_TYPE::dt_role:
		return true;
		break;
	default:
		break;
	}
	return false;
}
