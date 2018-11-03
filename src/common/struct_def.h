#pragma once
#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <unordered_map>

//类型定义
using VEC_STRING = std::vector<std::string>;

//服务器类型
enum class SRV_TYPE
{
	rt_gatesrv = 1,
	rt_gamesrv = 2,
	rt_gmsrv = 3,
	rt_routersrv = 4
};

//路由
struct ROUTER_UNIT
{
	std::string ip;
	int port;
	std::string type;

	std::string GenName() const;
};

struct ROUTER_INFO
{
	ROUTER_UNIT send;
	ROUTER_UNIT recv;

	static uint32_t GenRouterId(uint32_t srvtype, uint32_t idx);
	static uint32_t GetSrvType(uint32_t routerid);
	static uint32_t GetIndex(uint32_t routerid);

private:
	static const uint32_t s_bval = 1000;
};

using VEC_ROUTER_INFO = std::vector<ROUTER_INFO>;
using MAP_ROUTER_INFO = std::unordered_map<uint32_t, VEC_ROUTER_INFO>;
using MAP_INDEX_ROUTER_INFO = std::unordered_map<uint32_t, MAP_ROUTER_INFO>;


//消息
struct MSG_INNER_HEAD
{
	unsigned int src_srv_id = 0;
	unsigned int dst_srv_id = 0;
	unsigned int cmd = 0;
	unsigned int uid = 0;
};

struct MSG_INNER_INFO
{
	MSG_INNER_HEAD head;
	std::string data;

	std::string Encode() const;
	void Decode(const std::string& input);
	unsigned int DataSize() const { return data.size(); }
	unsigned int HeaderSize() const { return sizeof(MSG_INNER_HEAD); }
	unsigned int SrcId() const { return head.src_srv_id; }
	unsigned int DstId() const { return head.dst_srv_id; }
	unsigned int Cmd() const { return head.cmd; }
	unsigned int Uid() const { return head.uid; }
};

//数据库
enum class DB_TYPE
{
	dt_role = 1,
};

struct DB_REDIS_CONFIG_INFO
{
	std::string ip;
	int port;
	std::string passwd;
	uint32_t dbindex = 0;
};
using MAP_REDIS_DB_CONFIG = std::unordered_map<uint32_t, DB_REDIS_CONFIG_INFO>;
