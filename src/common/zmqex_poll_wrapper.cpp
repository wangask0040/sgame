#include "zmqex_poll_wrapper.h"
#include <assert.h>
#include "logging.h"

CZmqPollWrapper::CZmqPollWrapper()
{}

CZmqPollWrapper::~CZmqPollWrapper()
{
	if (_poll_lst != nullptr)
	{
		delete[] _poll_lst;
		_poll_lst = nullptr;
	}
}

bool CZmqPollWrapper::Init()
{
	auto& mapunit = CMsgMqWrapperSingleton::Instance()->GetMapUnit();
	_lst_size = mapunit.size();
	assert(_lst_size > 0);
	assert(_poll_lst == nullptr);

	_poll_lst = new zmq_pollitem_t[_lst_size];
	uint32_t pos = 0;
	for (auto& it : mapunit)
	{
		_poll_lst[pos].socket = it.second->GetRecvPtr();
		_poll_lst[pos].events = ZMQ_POLLIN;
		++pos;
	}
	return _lst_size > 0;
}

int CZmqPollWrapper::Wait(long timeout)
{
	auto ret = zmq_poll(_poll_lst, _lst_size, timeout);
	if (ret < 0)
		NLOGWARN << "|errno:" << errno << "|strerrno:" << strerror(errno)
		<< "|zstrerrno:" << CMsgMQUnit::ZmqStrError(errno);
	return ret;
}

void CZmqPollWrapper::Proc(int event_num)
{
	int cur_proc_num = 0;

	auto& mapunit = CMsgMqWrapperSingleton::Instance()->GetMapUnit();
	for (auto& it : mapunit)
	{
		if (cur_proc_num >= event_num)
			return;

		std::string data;
		uint32_t routerid = 0;
		
		bool recvret = false;
		while (it.second->Recv(data, routerid) > 0)
		{
			recvret = true;
			
			auto ptr = reinterpret_cast<const MSG_INNER_INFO*>(data.c_str());
			assert(it.first == ptr->SrcId());

			CMsgMqWrapperSingleton::Instance()
				->SendMsg(ptr->DstId(), data, ptr->SrcId(), ptr->Cmd());

			NLOGDEBUG << "src:" << ptr->SrcId() << "|dst:" << ptr->DstId()
				<< "|uid:" << ptr->Uid() << "|cmd:" << ptr->Cmd() 
				<< "|size:" << data.size() - ptr->HeaderSize();
		}

		if (recvret)
			cur_proc_num++;
	}
}