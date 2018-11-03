
#pragma once

#include <string>
#include "singleton.h"
#include "zmqex_wrapper.h"

class CZmqRouterClientWrapper 
{
public:
	int Init(const std::vector<ROUTER_INFO>& vec);
	bool RecvFrame(MSG_INNER_INFO& frame);
	uint32_t SendFrame(const MSG_INNER_INFO& frame);
	uint32_t GenRouterIdx(uint64_t uid) const;

	//////
	int Wait(long timeout);//∫¡√Î
	//////

protected:
	bool RecvFrame(CMsgMQUnit* ptr, MSG_INNER_INFO& frame);

private:
	MAP_MSGMQ_UNIT _msgUnit;
	std::vector<uint32_t> _vecRouterIdx;

	zmq_pollitem_t* _poll_lst = nullptr;
	uint32_t _lst_size = 0;
};

using CZmqRouterClientWrapperSgt = Singleton<CZmqRouterClientWrapper>;
