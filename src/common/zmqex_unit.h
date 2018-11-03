#pragma once
#include "struct_def.h"
#include <zmq.h>
#include <map>
#include "singleton.h"
#include <unordered_map>

class CMsgMQUnit
{
public:
	CMsgMQUnit();
	~CMsgMQUnit();
	bool Bind(const ROUTER_INFO& st);
	bool Connect(const ROUTER_INFO& st);
	int Recv(std::string& data, uint32_t& routerid);
	int Send(const std::string& data, uint32_t routerid);
	void* GetRecvPtr() { return _recvsock; }
	static const char* ZmqStrError(int en);

protected:

private:
	void* _ctx;
	void* _sendsock;
	void* _recvsock;	
};

using MAP_MSGMQ_UNIT = std::unordered_map<uint32_t, CMsgMQUnit*>;

using CMsgMQUnitSingleton = Singleton<CMsgMQUnit>;