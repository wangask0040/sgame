#include "logging.h"
#include "zmqex_connect_wrapper.h"
#include <string.h>
#include <arpa/inet.h>
#include "randomex.h"
#include <assert.h>


int CZmqRouterClientWrapper::Init(const std::vector<ROUTER_INFO>& vec)
{
	for (size_t i = 0; i < vec.size(); ++i)
	{
		_msgUnit[i] = new CMsgMQUnit();
		auto ret = _msgUnit[i]->Connect(vec[i]);
		assert(ret);
		if (!ret)
			return -1;
	}

	for (auto& it : _msgUnit)
	{
		_vecRouterIdx.push_back(it.first);
	}

	//ÓÃzmq_pollÀ´¼àÌý
	_lst_size = _msgUnit.size();
	_poll_lst = new zmq_pollitem_t[_lst_size];
	int pos = 0;
	for (auto& it : _msgUnit)
	{
		_poll_lst[pos].socket = it.second->GetRecvPtr();
		_poll_lst[pos].events = ZMQ_POLLIN;
		++pos;
	}

	return vec.size() > 0 ? 0 : -1;
}

bool CZmqRouterClientWrapper::RecvFrame(MSG_INNER_INFO& frame)
{
	static size_t c = 0;
	size_t pos = c % _vecRouterIdx.size();
	c++;

	for (size_t i = pos; i < _vecRouterIdx.size(); ++i)
	{
		if (RecvFrame(_msgUnit[_vecRouterIdx[i]], frame))
			return true;
	}

	for (size_t i = 0; i < pos; ++i)
	{
		if (RecvFrame(_msgUnit[_vecRouterIdx[i]], frame))
			return true;
	}
	
	return false;
}

uint32_t CZmqRouterClientWrapper::SendFrame(const MSG_INNER_INFO& frame)
{
	auto routerid = frame.DstId();
	auto i = GenRouterIdx(frame.Uid());
	auto size = _msgUnit[i]->Send(frame.Encode(), routerid);
	return size;
}

uint32_t CZmqRouterClientWrapper::GenRouterIdx(uint64_t uid) const
{
	auto i = uid % _vecRouterIdx.size();
	return _vecRouterIdx[i];
}

bool CZmqRouterClientWrapper::RecvFrame(CMsgMQUnit* ptr, MSG_INNER_INFO& frame)
{
	std::string data;
	uint32_t routerid = 0;
	auto size = ptr->Recv(data, routerid);
	assert(size == static_cast<int>(data.size()));
	if (size > 0)
	{
		assert(size >= static_cast<int>(frame.HeaderSize()));
		frame.Decode(data);
		return true;
	}
	else
		return false;
}

int CZmqRouterClientWrapper::Wait(long timeout)
{
	auto ret = zmq_poll(_poll_lst, _lst_size, timeout);
// 	if (ret < 0)
// 		NLOGWARN << "errno:" << errno << "|strerrno:" << strerror(errno)
// 		<< "|zstrerrno:" << CMsgMQUnit::ZmqStrError(errno);
	return ret;
}
