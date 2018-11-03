#include "dbconfig.h"
#include <assert.h>
#include "server_config.h"

bool CDbConfig::Read(tinyxml2::XMLElement* root)
{
	for (tinyxml2::XMLElement* item = root->FirstChildElement(); item != nullptr; item = item->NextSiblingElement())
	{
		auto dbtype = item->UnsignedAttribute("dbtype");
		if (!CServerConfig::IsDbType(dbtype))
		{
			assert(false);
			return false;
		}

		DB_REDIS_CONFIG_INFO st;
		st.ip = item->Attribute("ip");
		st.port = item->IntAttribute("port");
		st.passwd = item->Attribute("passwd");
		st.dbindex = item->UnsignedAttribute("redis_db_index");
		auto ret = _dbConfig.insert(std::make_pair(dbtype, st));
		assert(ret.second);
	}
	return _dbConfig.size() > 0;
}

const DB_REDIS_CONFIG_INFO* CDbConfig::GetInfoPtr(int type) const
{
	auto it = _dbConfig.find(type);
	if (it != _dbConfig.end())
		return &it->second;
	else
		return nullptr;
}
