#include "tcp_server.h"
#include <event2/buffer.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "logging.h"
#include <assert.h>

ITcpServer::ITcpServer()
{
	_base = event_base_new();
}

ITcpServer::~ITcpServer()
{
	if (_listener != nullptr)
		evconnlistener_free(_listener);

	event_base_free(_base);
}

bool ITcpServer::setaddr(const std::string& addr)
{
	sockaddr staddr;
	int len = sizeof(staddr);
	auto ret = evutil_parse_sockaddr_port(addr.c_str(), &staddr, &len);
	if (ret != 0)
	{
		assert(false);
		return false;
	}

	_listener = evconnlistener_new_bind(_base, ITcpServer::OnAccept, this,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1, &staddr, len);

	if (_listener == nullptr)
	{
		assert(false);
		return false;
	}

	NLOGINFO << "server init!" << addr;
	return true;
}

void ITcpServer::OnAccept(evconnlistener* /*lst*/, evutil_socket_t fd, sockaddr* sock, int /*socklen*/, void* ctx)
{
	ITcpServer* p = reinterpret_cast<ITcpServer*>(ctx);
	auto base = p->base();

	bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, ITcpServer::OnRead, nullptr, ITcpServer::OnEvent, ctx);
	bufferevent_enable(bev, EV_READ);

	auto paddr = reinterpret_cast<sockaddr_in*>(sock);
	char ip[32];
	evutil_inet_ntop(AF_INET, &paddr->sin_addr, ip, sizeof(ip));

	auto tmpfd = bufferevent_getfd(bev);
	NLOGDEBUG << "connect accept from:" << ip << "|fd:" << tmpfd;

	p->AddBev(bev);

	p->OnTcpConnented(tmpfd, *paddr);
}

void ITcpServer::OnRead(bufferevent* bev, void* ctx)
{
	while (true)
	{
		ITcpServer* p = reinterpret_cast<ITcpServer*>(ctx);
		auto headlen = p->HeadSize();

		auto evbuf = bufferevent_get_input(bev);
		if (evbuffer_get_length(evbuf) < headlen)
			return;

		char* headbuf = new char[headlen];
		evbuffer_copyout(evbuf, headbuf, headlen);
		auto packlen = p->DecodeHead(headbuf, headlen);
		delete[] headbuf;

		auto totallen = packlen + headlen;
		if (evbuffer_get_length(evbuf) < totallen)
			return;

		char* buftotal = new char[totallen];
		evbuffer_remove(evbuf, buftotal, totallen);
		auto fd = bufferevent_getfd(bev);
		p->OnTcpRead(buftotal, totallen, fd);
		delete[] buftotal;
	}
}

void ITcpServer::WriteData(int fd, const char* buf, unsigned int len)
{
	auto it = _mapbev.find(fd);
	if (it != _mapbev.end())
	{
		auto p = it->second;
        if (bufferevent_write(p, buf, len) < 0) {
            NLOGERROR << "fd:" << fd << "|len:" << len << "| bufferevent_write failed.";
        }
	}
}

void ITcpServer::OnEvent(bufferevent* bev, short what, void* ctx)
{
	auto p = reinterpret_cast<ITcpServer*>(ctx);
	auto fd = bufferevent_getfd(bev);
	if (what & BEV_EVENT_EOF)
	{
		NLOGINFO << "fd:" << fd << "|Connection closed";
		p->OnTcpDisConnect(fd);
		p->RemoveBev(bev);
	}
	else if (what & BEV_EVENT_ERROR)
	{
		NLOGDEBUG << "fd:" << fd << "|Got an error on the connection: " << strerror(errno);
		p->OnTcpDisConnect(fd);
		p->RemoveBev(bev);
	}
	else if (what & BEV_EVENT_CONNECTED)
	{
		NLOGINFO << "fd:" << fd << "|Connection connected";
	}
	else
	{
		NLOGINFO << "fd:" << fd << "|None of the other events can happen here, since we haven't enabled timeouts";
	}
}

void ITcpServer::OnTcpDisConnect(int /*fd*/)
{

}

void ITcpServer::dispath()
{
	NLOGINFO << "tcp server start!";
	event_base_dispatch(_base);
}

void ITcpServer::OnTcpConnented(int /*fd*/, const sockaddr_in& /*addr*/)
{

}

void ITcpServer::AddBev(bufferevent* bev)
{
	auto fd = bufferevent_getfd(bev);
	_mapbev[fd] = bev;
}

void ITcpServer::RemoveBev(bufferevent* bev)
{
	auto fd = bufferevent_getfd(bev);
	bufferevent_free(bev);
	_mapbev.erase(fd);
}

void ITcpServer::RemoveBev(int fd)
{
	auto it = _mapbev.find(fd);
	if (it != _mapbev.end())
		RemoveBev(it->second);
}

void ITcpServer::OnExit() const
{
    std::cout << "Tcp server exit." << std::endl;
    event_base_loopexit(_base, nullptr);
}
