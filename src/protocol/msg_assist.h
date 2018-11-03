#pragma once
#include "msg_info.pb.h"
#include <string>

class CMsgAssist
{
public:
	CMsgAssist() {}
	~CMsgAssist() {}

	static unsigned int decode(const char* data, unsigned int len, protocol::msg_info& info, bool& msg_decode_ret);
	static void encode(const protocol::msg_info& info, std::string& data);
};