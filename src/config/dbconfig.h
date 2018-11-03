#pragma once
#include "loadxml.h"
#include "common/singleton.h"
#include "common/struct_def.h"
#include <map>


class CDbConfig
	: public ILoadXml
{
public:
	CDbConfig() {}
	~CDbConfig() {}
	virtual bool Read(tinyxml2::XMLElement* root) override;
	const MAP_REDIS_DB_CONFIG& Get() const { return _dbConfig; }
	const DB_REDIS_CONFIG_INFO* GetInfoPtr(int type) const;

private:
	void Clear() override {}
	MAP_REDIS_DB_CONFIG _dbConfig;
};


using CDbConfigSgt = Singleton<CDbConfig>;
