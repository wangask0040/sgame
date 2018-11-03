#include "msg_assist.h"
#include <arpa/inet.h> 

using namespace protocol;

static const unsigned int MSG_HEAD_FLAG = sizeof(unsigned int);

unsigned int CMsgAssist::decode(const char* data, unsigned int len, protocol::msg_info& info, bool& msg_decode_ret)
{
	//包的前四个字节为整个包的长度
	if (len < MSG_HEAD_FLAG)
		return 0;

	auto p = (const unsigned int*)(data);
	auto msg_len = ntohl(*p);
	if (len < (msg_len + MSG_HEAD_FLAG))
		return 0;
	
	msg_decode_ret = info.ParseFromArray(data + MSG_HEAD_FLAG, msg_len);
	return msg_len + MSG_HEAD_FLAG;
}

void CMsgAssist::encode(const protocol::msg_info& info, std::string& data)
{
	std::string d;
	if (!info.SerializeToString(&d))
		return;

	auto msg_len = d.size();
	auto n = htonl(msg_len);
	data.assign((const char*)(&n), MSG_HEAD_FLAG);
	data += d;
}