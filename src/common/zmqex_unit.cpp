#include "zmqex_unit.h"
#include <assert.h>
#include <cstring>
#include "logging.h"

CMsgMQUnit::CMsgMQUnit()
{
	_ctx = zmq_ctx_new();
	assert(_ctx);

	_sendsock = zmq_socket(_ctx, ZMQ_PUSH);
	assert(_sendsock != nullptr);

	_recvsock = zmq_socket(_ctx, ZMQ_PULL);
	assert(_recvsock != nullptr);

}

CMsgMQUnit::~CMsgMQUnit()
{
	zmq_close(_recvsock);
	zmq_close(_sendsock);
	zmq_ctx_term(_ctx);
}

bool CMsgMQUnit::Bind(const ROUTER_INFO& st)
{
	auto sendname = st.send.GenName();
	auto ret = zmq_bind(_sendsock, sendname.c_str());
	assert(ret == 0);

	auto recvname = st.recv.GenName();
	ret = zmq_bind(_recvsock, recvname.c_str());
	assert(ret == 0);
	return true;
}

bool CMsgMQUnit::Connect(const ROUTER_INFO& st)
{
	auto recvname = st.recv.GenName();
	auto ret = zmq_connect(_sendsock, recvname.c_str());
	assert(ret == 0);
	auto sendname = st.send.GenName();
	ret = zmq_connect(_recvsock, sendname.c_str());
	assert(ret == 0);
	return true;
}

int CMsgMQUnit::Recv(std::string& data, uint32_t& /*routerid*/)
{
	zmq_msg_t recvmsg;
	auto ret = zmq_msg_init(&recvmsg);
	assert(ret == 0);

	zmq_msg_recv(&recvmsg, _recvsock, ZMQ_DONTWAIT);
	auto len = zmq_msg_size(&recvmsg);
	if (len > 0)
	{
		void* d = zmq_msg_data(&recvmsg);
		data.assign(reinterpret_cast<char*>(d), len);
	}

	zmq_msg_close(&recvmsg);

	return len;
}

int CMsgMQUnit::Send(const std::string& data, uint32_t /*routerid*/)
{
	zmq_msg_t sendmsg;
	auto ret = zmq_msg_init(&sendmsg);
	assert(ret == 0);

	int len = data.size();
	ret = zmq_msg_init_size(&sendmsg, len);
	assert(ret == 0);
	memcpy(zmq_msg_data(&sendmsg), data.c_str(), len);
	len = zmq_msg_send(&sendmsg, _sendsock, ZMQ_DONTWAIT);

	if (len != (int)data.size())
	{
		NLOGWARN << "len:" << len << "|data size:" << data.size() 
			<< "|errno:" << errno << "|strerrno:" << strerror(errno)
			<< "|zstrerrno:" << ZmqStrError(errno);
	}

	zmq_msg_close(&sendmsg);
	return len;
}

const char* CMsgMQUnit::ZmqStrError(int en)
{
	return zmq_strerror(en);
}
