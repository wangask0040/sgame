#include "struct_def.h"

std::string ROUTER_UNIT::GenName() const
{
	std::stringstream ss;
	ss << type << "://"
		<< ip << ":" << port;
	return ss.str();
}

uint32_t ROUTER_INFO::GenRouterId(uint32_t srvtype, uint32_t idx)
{
	return s_bval * srvtype + idx;
}

uint32_t ROUTER_INFO::GetSrvType(uint32_t routerid)
{
	return routerid / s_bval;
}

uint32_t ROUTER_INFO::GetIndex(uint32_t routerid)
{
	return routerid % s_bval;
}

std::string MSG_INNER_INFO::Encode() const
{
	std::string tmp;
	tmp.assign(reinterpret_cast<const char*>(&head), HeaderSize());
	tmp.append(data);
	return tmp;
}

void MSG_INNER_INFO::Decode(const std::string& input)
{
	const MSG_INNER_HEAD* p = reinterpret_cast<const MSG_INNER_HEAD*>(input.c_str());
	data.assign(input.c_str() + HeaderSize(), input.size() - HeaderSize());
}