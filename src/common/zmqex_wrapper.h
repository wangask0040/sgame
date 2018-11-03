#pragma once
#include "singleton.h"
#include "zmqex_unit.h"

class CRouterMsgMqWrapper
{
public:
	CRouterMsgMqWrapper();
	~CRouterMsgMqWrapper() {}
	bool Init(const MAP_ROUTER_INFO& cfg);
	bool SendMsg(uint32_t routerid, const std::string& data, uint32_t fromrouterid, int cmd);
	MAP_MSGMQ_UNIT& GetMapUnit() { return _msgmqUnit; }
	

private:
	MAP_MSGMQ_UNIT _msgmqUnit;
};

using CMsgMqWrapperSingleton = Singleton<CRouterMsgMqWrapper>;