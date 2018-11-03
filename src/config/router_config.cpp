#include "router_config.h"
#include <assert.h>
#include "server_config.h"

bool CRouterConfig::Read(tinyxml2::XMLElement* root)
{
	auto type = root->Attribute("type");
	uint32_t idx = 0;
	for (auto im = root->FirstChildElement(); im != nullptr; im = im->NextSiblingElement())
	{
		auto& iff = _routerIndexInfo[idx];
		for (auto itemsrv = im->FirstChildElement(); itemsrv != nullptr; itemsrv = itemsrv->NextSiblingElement())
		{
			auto srvtype = itemsrv->UnsignedAttribute("srvtype");
			if (!CServerConfig::IsSrvType(srvtype))
			{
				assert(false);
				return false;
			}

			auto ip = itemsrv->Attribute("ip");
			for (auto item = itemsrv->FirstChildElement(); item != nullptr; item = item->NextSiblingElement())
			{
				ROUTER_INFO info;
				ROUTER_UNIT& stsend = info.send;
				stsend.ip = ip;
				stsend.port = item->IntAttribute("send");
				stsend.type = type;

				ROUTER_UNIT& strecv = info.recv;
				strecv.ip = ip;
				strecv.port = item->IntAttribute("recv");
				strecv.type = type;

				iff[srvtype].push_back(info);
			}
		}

		idx++;
	}

	return _routerIndexInfo.size() > 0;
}

uint32_t CRouterConfig::GenRouterId(uint32_t srvtype, uint64_t uid) const
{
	auto it = _routerIndexInfo.begin()->second.find(srvtype);
	auto idx = uid % it->second.size();
	return ROUTER_INFO::GenRouterId(srvtype, idx);
}


const MAP_ROUTER_INFO* CRouterConfig::GetByRouterSrvIdx(uint32_t idx) const
{
	auto it = _routerIndexInfo.find(idx);
	if (it != _routerIndexInfo.end())
		return &it->second;
	else
		return nullptr;
}

const ROUTER_INFO* CRouterConfig::GetInfoPtrByRouterSrvIdx(uint32_t idx, uint32_t routerid) const
{
	auto it = _routerIndexInfo.find(idx);
	if (it != _routerIndexInfo.end())
	{
		auto srvtype = ROUTER_INFO::GetSrvType(routerid);
		auto x = ROUTER_INFO::GetIndex(routerid);

		auto ittt = it->second.find(srvtype);
		if (ittt != it->second.end())
			return &(ittt->second[x]);
		else
			return nullptr;
	}
	else
		return nullptr;
}

uint32_t CRouterConfig::GenRouterIdByRouterSrvIdx(uint32_t idx, uint32_t srvtype, uint64_t uid) const
{
	auto it = _routerIndexInfo.find(idx);
	if (it != _routerIndexInfo.end())
	{
		auto ittt = it->second.find(srvtype);
		if (ittt != it->second.end())
		{
			auto x = uid % ittt->second.size();
			return ROUTER_INFO::GenRouterId(srvtype, x);
		}
		else
			return 0;
	}
	else
		return 0;
}

const MAP_ROUTER_INFO* CRouterConfig::Get(uint32_t routeridx) const
{
	auto it = _routerIndexInfo.find(routeridx);
	if (it != _routerIndexInfo.end())
	{
		return &it->second;
	}
	else
		return nullptr;
}

void CRouterConfig::GetRouterConnectVec(VEC_ROUTER_INFO& vec, unsigned int routerid) const
{
	auto maxridx = CRouterConfigSgt::Instance()->MaxRouterIdx();
	for (uint32_t i = 0; i < maxridx; ++i)
	{
		auto ptr = CRouterConfigSgt::Instance()->GetInfoPtrByRouterSrvIdx(i, routerid);
		assert(ptr != nullptr);
		vec.push_back(*ptr);
	}
}
