#pragma once
#include "common/singleton.h"
#include "loadxml.h"
#include "common/struct_def.h"


class CRouterConfig
	: public ILoadXml
{
public:
	CRouterConfig() {}
	~CRouterConfig() {}
	virtual bool Read(tinyxml2::XMLElement* root) override;
	const MAP_ROUTER_INFO* Get(uint32_t routeridx) const;
	uint32_t GenRouterId(uint32_t srvtype, uint64_t uid) const;

	const MAP_ROUTER_INFO* GetByRouterSrvIdx(uint32_t idx) const;
	const ROUTER_INFO* GetInfoPtrByRouterSrvIdx(uint32_t idx, uint32_t routerid) const;
	uint32_t GenRouterIdByRouterSrvIdx(uint32_t idx, uint32_t srvtype, uint64_t uid) const;
	uint32_t MaxRouterIdx() const { return _routerIndexInfo.size(); }
	
	void GetRouterConnectVec(VEC_ROUTER_INFO& vec, unsigned int routerid) const;

private:
	virtual void Clear() override {}
	MAP_INDEX_ROUTER_INFO _routerIndexInfo;
};

using CRouterConfigSgt = Singleton<CRouterConfig>;
