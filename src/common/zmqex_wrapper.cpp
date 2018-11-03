#include "zmqex_wrapper.h"
#include <assert.h>
#include "logging.h"


CRouterMsgMqWrapper::CRouterMsgMqWrapper()
{
}

bool CRouterMsgMqWrapper::Init(const MAP_ROUTER_INFO& cfg)
{
	for (auto& it : cfg)
	{
		auto srvtype = it.first;
		auto& vec = it.second;

		for (size_t i = 0; i < vec.size(); ++i)
		{
			CMsgMQUnit* ptr = new CMsgMQUnit();
			auto routerid = ROUTER_INFO::GenRouterId(srvtype, i);
			auto ret = _msgmqUnit.insert(std::make_pair(routerid, ptr));
			assert(ret.second);
			
			auto& unit = _msgmqUnit[routerid];
			auto bret = unit->Bind(vec[i]);
			assert(bret);
		}
	}

	return true;
}

bool CRouterMsgMqWrapper::SendMsg(uint32_t routerid, const std::string& data, uint32_t fromrouterid, int cmd)
{
	auto it = _msgmqUnit.find(routerid);
	if (it != _msgmqUnit.end())
	{
		return it->second->Send(data, fromrouterid) > 0;
	}
	else
	{
		NLOGWARN << "src:" << fromrouterid << "|dst:" << routerid << "|cmd:" << cmd;
		return false;
	}
}

