#pragma once

#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>

#include <string>
#include <map>


enum WebSocketFrameType
{
	ERROR_FRAME = 0xFF00,
	INCOMPLETE_FRAME = 0xFE00,

	OPENING_FRAME = 0x3300,
	CLOSING_FRAME = 0x3400,

	INCOMPLETE_TEXT_FRAME = 0x01,
	INCOMPLETE_BINARY_FRAME = 0x02,

	TEXT_FRAME = 0x81,
	BINARY_FRAME = 0x82,

	PING_FRAME = 0x19,
	PONG_FRAME = 0x1A
};

enum WebSocketConnectState
{
	WS_HANDESHAKE = 1,
	WS_CONNECTED = 2,
};

class IWebSocketSever
{
public:
	IWebSocketSever();
	virtual ~IWebSocketSever();

	void start(const std::string& addr);
	event_base* base() { return _base; }

	static void OnAccept(evconnlistener* lst, evutil_socket_t fd, sockaddr* sock, int socklen, void* ctx);
	static void OnRead(bufferevent* bev, void* ctx);
	static void OnEvent(bufferevent* bev, short what, void* ctx);
	static int GetFrame(unsigned char* inbuffer, int inlength, std::string& output, unsigned int& framelen);
	static int MakeFrame(int frametype, const unsigned char* msg, int msglen, unsigned char* buffer);
	static void WriteData(int frametype, const std::string& data, void* ctx);

	virtual void OnWSDisconnect(bufferevent* bev);
	virtual void OnWSRead(const std::string& input, bufferevent* bev) = 0;
	virtual void OnWSConnected(bufferevent* bev);

	void ProcHandShake(const std::string& head, std::string& answer);
	void SetWSState(evutil_socket_t fd, int state);
	void DelWSState(evutil_socket_t fd);
	int GetWSState(evutil_socket_t fd) const;


protected:

private:
	event_base* _base = nullptr;
	evconnlistener* _listener = nullptr;
	std::map<evutil_socket_t, int> _connstate;

	static const char* s_headendflag;
	static const char* s_sep1;
	static const char* s_sep2;
	static const char* s_magickey;
	static const int s_maxwebsocketbufsize;
};
