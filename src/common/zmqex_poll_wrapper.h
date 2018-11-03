#pragma once
#include "zmqex_wrapper.h"
#include "singleton.h"

class CZmqPollWrapper
{
public:
	CZmqPollWrapper();
	~CZmqPollWrapper();
	bool Init();
	int Wait(long timeout);//∫¡√Î
	void Proc(int event_num);

private:
	zmq_pollitem_t* _poll_lst = nullptr;
	uint32_t _lst_size = 0;
};

using CZmqPollWrapperSingleton = Singleton<CZmqPollWrapper>;
