#pragma once
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <map>
#include <string>
#include <unordered_map>

using MAP_FD_BUFFEREVENT = std::unordered_map<int, bufferevent*>;

class ITcpServer
{
public:
	ITcpServer();
	virtual ~ITcpServer();

	bool setaddr(const std::string& addr);
	void dispath();
	event_base* base() { return _base; }
	void WriteData(int fd, const char* buf, unsigned int len);

	static void OnAccept(evconnlistener* lst, evutil_socket_t fd,
		sockaddr* sock, int socklen, void* ctx);
	static void OnRead(bufferevent* bev, void* ctx);
	static void OnEvent(bufferevent* bev, short what, void* ctx);

	virtual void OnTcpConnented(int fd, const sockaddr_in& addr);
	virtual void OnTcpDisConnect(int fd);
	virtual unsigned int HeadSize() const = 0;
	virtual unsigned int DecodeHead(char* buf, unsigned int len) const = 0;
	virtual void OnTcpRead(char* buf, unsigned int len, int fd) = 0;
	void RemoveBev(int fd);

protected:
	void AddBev(bufferevent* bev);
	void RemoveBev(bufferevent* bev);
    void OnExit() const;

private:
	event_base* _base = nullptr;
	evconnlistener* _listener = nullptr;
	MAP_FD_BUFFEREVENT _mapbev;
};


