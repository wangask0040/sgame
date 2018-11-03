#include "websocket_sever.h"
#include "event2/buffer.h"
#include "logging.h"
#include "stringex.h"
#include <unordered_map>
#include "sha1.h"
#include "base64.h"

const char* IWebSocketSever::s_headendflag = "\r\n\r\n";
const char* IWebSocketSever::s_sep1 = "\r\n";
const char* IWebSocketSever::s_sep2 = ": ";
const char* IWebSocketSever::s_magickey = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
const int IWebSocketSever::s_maxwebsocketbufsize = 65535;//0xFFFF


IWebSocketSever::IWebSocketSever()
{
	_base = event_base_new();
}

IWebSocketSever::~IWebSocketSever()
{
	if (_listener != nullptr)
		evconnlistener_free(_listener);

	event_base_free(_base);
}

void IWebSocketSever::start(const std::string& addr)
{
	sockaddr staddr;
	int len = sizeof(staddr);
	evutil_parse_sockaddr_port(addr.c_str(), &staddr, &len);

	evconnlistener_new_bind(_base, IWebSocketSever::OnAccept, this,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1, &staddr, len);

	NLOGDEBUG << "server start!" << addr;
	event_base_dispatch(_base);
}


void IWebSocketSever::OnAccept(evconnlistener* /*lst*/, evutil_socket_t fd, sockaddr* sock, int /*socklen*/, void* ctx)
{
	IWebSocketSever* p = reinterpret_cast<IWebSocketSever*>(ctx);
	auto base = p->base();

	bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, IWebSocketSever::OnRead, nullptr, IWebSocketSever::OnEvent, ctx);
	bufferevent_enable(bev, EV_READ);

	auto paddr = reinterpret_cast<sockaddr_in*>(sock);
	char ip[32];
	evutil_inet_ntop(AF_INET, &paddr->sin_addr, ip, sizeof(ip));
	NLOGDEBUG << "connect accept from:" << ip;

	//连接到来设置为握手状态
	p->SetWSState(fd, WS_HANDESHAKE);

	p->OnWSConnected(bev);
}

void IWebSocketSever::SetWSState(evutil_socket_t fd, int state)
{
	auto it = _connstate.find(fd);
	if (it != _connstate.end())
		it->second = state;
	else
		_connstate[fd] = state;
}

void IWebSocketSever::DelWSState(evutil_socket_t fd)
{
	auto it = _connstate.find(fd);
	if (it != _connstate.end())
		_connstate.erase(it);
}

int IWebSocketSever::GetWSState(evutil_socket_t fd) const
{
	auto it = _connstate.find(fd);
	if (it != _connstate.end())
		return it->second;
	else
		return 0;
}

void IWebSocketSever::ProcHandShake(const std::string& head, std::string& answer)
{
	std::vector<std::string> vec1;
	split(head, s_sep1, vec1);
	std::unordered_map<std::string, std::string> mapkey;

	for (auto& it : vec1)
	{
		std::vector<std::string> vec2;
		split(it, s_sep2, vec2);
		if (vec2.size() > 1)
		{
			mapkey[vec2[0]] = vec2[1];
		}
	}

	auto it = mapkey.find("Sec-WebSocket-Key");
	if (it == mapkey.end())
	{
		NLOGERROR << "Sec-WebSocket-Key is null!";
		return;
	}

	answer += "HTTP/1.1 101 Switching Protocols";
	answer += s_sep1;

	answer += "Upgrade";
	answer += s_sep2;
	answer += "WebSocket";
	answer += s_sep1;

	answer += "Connection";
	answer += s_sep2;
	answer += "Upgrade";
	answer += s_sep1;

	if (it->second.size() > 0)
	{
		std::string accept_key;
		accept_key += it->second;
		accept_key += s_magickey;

		SHA1 sha;
		sha.Input(accept_key.data(), accept_key.size());
		unsigned int digest[5];
		sha.Result(digest);

		for (int i = 0; i < 5; i++)
			digest[i] = htonl(digest[i]);

		auto sever_key = base64_encode(reinterpret_cast<unsigned char*>(digest), 20);

		answer += "Sec-WebSocket-Accept";
		answer += s_sep2;
		answer += sever_key;
		answer += s_sep1;
	}

	answer += s_sep1;

	NLOGDEBUG << answer;
}

void IWebSocketSever::OnRead(bufferevent* bev, void* ctx)
{
	IWebSocketSever* p = reinterpret_cast<IWebSocketSever*>(ctx);
	auto fd = bufferevent_getfd(bev);
	auto evbuf = bufferevent_get_input(bev);
	auto len = evbuffer_get_length(evbuf);
	NLOGDEBUG << "evbuffer len:" << len;

	if (p->GetWSState(fd) == WS_HANDESHAKE)
	{
		if (len > 0)
		{
			char* tmpbuf = new char[len];
			evbuffer_copyout(evbuf, tmpbuf, len);
			NLOGDEBUG << tmpbuf;

			std::string head(tmpbuf, len);
			delete[] tmpbuf;

			auto pos = head.find(s_headendflag);
			if (pos == std::string::npos)
				return;
			else
				len = pos + strlen(s_headendflag);

			std::string output;
			p->ProcHandShake(head, output);

			evbuffer_drain(evbuf, len);
			bufferevent_write(bev, output.c_str(), output.size());

			p->SetWSState(fd, WS_CONNECTED);
		}

		NLOGDEBUG << "after handshake evbuffer len:" << evbuffer_get_length(evbuf);

		return;
	}
	else
	{
		unsigned char* tmpbuf = new unsigned char[len];
		evbuffer_copyout(evbuf, tmpbuf, len);

		std::string input;
		unsigned int framelen = 0;
		auto frametype = GetFrame(tmpbuf, len, input, framelen);

		delete[] tmpbuf;

		std::stringstream ss;
		ss << "recv frametype:" << std::hex << frametype 
			<< std::oct << "|framelen:" << framelen;
		NLOGDEBUG << ss.str();

		if ((frametype == TEXT_FRAME || frametype == BINARY_FRAME)
			&& (len >= framelen))
		{
			evbuffer_drain(evbuf, framelen);

			p->OnWSRead(input, bev);

			//test
			//WriteData(frametype, input, bev);
			input = "21231";
			WriteData(frametype, input, bev);
		}
		else if (frametype == PING_FRAME || frametype == PONG_FRAME)
			evbuffer_drain(evbuf, framelen);
		else if (frametype == ERROR_FRAME)
		{
			p->OnWSDisconnect(bev);
			bufferevent_free(bev);
		}
	}
}

void IWebSocketSever::WriteData(int frametype, const std::string& data, void* ctx)
{
	auto size = data.size();
	unsigned char* buf = new unsigned char[size + 16];
	auto framelen = MakeFrame(frametype, reinterpret_cast<const unsigned char*>(data.c_str()), size, buf);
	NLOGDEBUG << "send frametype:" << std::hex << frametype 
		<< std::oct << "|framelen:" << framelen;
	auto bev = reinterpret_cast<bufferevent*>(ctx);
	bufferevent_write(bev, buf, framelen);
	delete[] buf;
}

void IWebSocketSever::OnEvent(bufferevent* bev, short what, void* ctx)
{
	auto p = reinterpret_cast<IWebSocketSever*>(ctx);
	if (what & BEV_EVENT_EOF)
	{
		NLOGDEBUG << "Connection closed";
		p->OnWSDisconnect(bev);
	}
	else if (what & BEV_EVENT_ERROR)
	{
		NLOGDEBUG << "Got an error on the connection: " << strerror(errno);
		p->OnWSDisconnect(bev);
	}
	else if (what & BEV_EVENT_CONNECTED)
	{
		NLOGDEBUG << "Connection connected";
	}
	else
	{
		NLOGDEBUG << "None of the other events can happen here, since we haven't enabled timeouts";
	}
}

void IWebSocketSever::OnWSConnected(bufferevent* /*bev*/)
{

}

void IWebSocketSever::OnWSDisconnect(bufferevent* bev)
{
	auto fd = bufferevent_getfd(bev);
	DelWSState(fd);
	NLOGDEBUG << "del fd:" << fd;
}

int IWebSocketSever::MakeFrame(int frametype, const unsigned char* msg, int msglen, unsigned char* buffer)
{
	int pos = 0;
	int size = msglen;
	buffer[pos++] = (unsigned char)frametype; // text frame

	if (size <= 125)
		buffer[pos++] = size;
	else if (size <= s_maxwebsocketbufsize)
	{
		buffer[pos++] = 126; //16 bit length follows
		buffer[pos++] = (size >> 8) & 0xFF; // leftmost first
		buffer[pos++] = size & 0xFF;
	}
	else
	{
		// >2^16-1 (65535)
		buffer[pos++] = 127; //64 bit length follows
							 // write 8 bytes length (significant first)
							 // since msg_length is int it can be no longer than 4 bytes = 2^32-1
							 // padd zeroes for the first 4 bytes
		for (int i = 3; i >= 0; i--)
			buffer[pos++] = 0;

		// write the actual 32bit msg_length in the next 4 bytes
		for (int i = 3; i >= 0; i--)
			buffer[pos++] = ((size >> 8 * i) & 0xFF);
	}

	memcpy((void*)(buffer + pos), msg, size);
	return (size + pos);
}

int IWebSocketSever::GetFrame(unsigned char* inbuffer, int inlength, std::string& output, unsigned int& framelen)
{
	if (inlength < 3)
		return INCOMPLETE_FRAME;

	unsigned char msg_opcode = inbuffer[0] & 0x0F;
	unsigned char msg_fin = (inbuffer[0] >> 7) & 0x01;
	unsigned char msg_masked = (inbuffer[1] >> 7) & 0x01;

	std::stringstream ss;
	ss << "opcode:" << std::hex << static_cast<int>(msg_opcode)
		<< "|fin:" << static_cast<int>(msg_fin)
		<< "|mask:" << static_cast<int>(msg_masked);
	NLOGDEBUG << ss.str();

	// *** message decoding 

	int pos = 2;
	int payload_length = inbuffer[1] & 0x7F;

	if (payload_length == 126)
	{
		//msglen is 16bit!
		uint16_t length = 0;
		memcpy(&length, inbuffer + pos, 2);
		pos += 2;
		payload_length = ntohs(length);
	}
	else if (payload_length == 127)
	{
		//msglen is 64bit!
		uint32_t length = 0;
		memcpy(&length, inbuffer + pos, 4);
		pos += 4;
		payload_length = ntohl(length);
	}

	if (inlength < payload_length + pos)
		return INCOMPLETE_FRAME;

	unsigned int mask = 0;
	if (msg_masked)
	{
		mask = *((unsigned int*)(inbuffer + pos));
		pos += 4;

		// unmask data:
		unsigned char* c = inbuffer + pos;
		for (int i = 0; i < payload_length; i++)
			c[i] = (c[i] ^ ((unsigned char*)(&mask))[i % 4]);
	}

	if (payload_length > 0)
		output = std::move(std::string(reinterpret_cast<char*>(inbuffer + pos),
			payload_length));

	framelen = (payload_length + pos);

	if (msg_opcode == 0x0)
		return (msg_fin) ? TEXT_FRAME : INCOMPLETE_TEXT_FRAME; // continuation frame ?
	else if (msg_opcode == 0x1)
		return (msg_fin) ? TEXT_FRAME : INCOMPLETE_TEXT_FRAME;
	else if (msg_opcode == 0x2)
		return (msg_fin) ? BINARY_FRAME : INCOMPLETE_BINARY_FRAME;
	else if (msg_opcode == 0x9)
		return PING_FRAME;
	else if (msg_opcode == 0xA)
		return PONG_FRAME;
	else if (msg_opcode == 0x8)
	{
		//连接关闭
		return ERROR_FRAME;
	}

	return ERROR_FRAME;
}

