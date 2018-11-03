#pragma once
#include <hiredis/hiredis.h>
#include <string>
#include <vector>
#include "struct_def.h"

struct HASH_UINT
{
	std::string field;
	std::string data;
};

using VEC_HASH_DATA = std::vector<HASH_UINT>;

class CRedsiClientUnit
{
public:
	CRedsiClientUnit();
	~CRedsiClientUnit();
	bool Init(const DB_REDIS_CONFIG_INFO& st);
	bool set(const std::string& key, const std::string& data);
	bool setnx(const std::string& key, const std::string& data);
	bool get(const std::string& key, std::string& data);
	bool get2(const std::string& key, std::string& data);
	uint64_t incr(const std::string& key);
	bool exists(const std::string& key);
	bool hgetall(const std::string& key, VEC_HASH_DATA& data);
	bool hset(const std::string& key, const std::string& field, const std::string& data);
	bool hsetnx(const std::string& key, const std::string& field, const std::string& data);
	bool hget(const std::string& key, const std::string& field, std::string& data);
	bool rpush(const std::string& key, const std::string& data);
	bool lrange(const std::string& key, int start, int end, VEC_STRING& vec);
	bool ltrim(const std::string& key, int start, int end);
	bool setex(const std::string& key, const std::string& data, uint32_t sec);
	bool zadd(const std::string& key, uint32_t score, const std::string& member);
	bool zrem(const std::string& key, const std::string& member);
	void zincrby(const std::string& key, uint32_t score, const std::string& member);
	int zrevrank(const std::string& key, const std::string& member);
	bool zrevrange(const std::string& key, int start, int end, VEC_STRING& vec);
	uint32_t zscore(const std::string& key, const std::string& member);
	bool zrangebyscore(const std::string& key, const std::string& minscore, const std::string& maxscore, int count, VEC_STRING& vec);
	bool del(const std::string& key);
	bool sadd(const std::string& key, const std::string& data);
	bool srem(const std::string& key, const std::string& data);
	void smembers(const std::string& key, VEC_STRING& vecdata);
	void keys(const std::string& key, VEC_STRING& vecdata);
	void mget(const VEC_STRING& key, VEC_STRING& data);

protected:
	void* cmdMode1(const char* cmd, const std::string& key);
	void* cmdMode2(const char* cmd, const std::string& key, const std::string& data);
	void* cmdMode3(const char* cmd, const std::string& key, const std::string& field, const std::string& data);
	void* cmdMode4(const char* cmd, const std::string& key, int start, int end);
	void* cmdMode5(const char* cmd, const std::string& key, uint32_t sec, const std::string& data);
	void* cmdMode6(const char* cmd, const std::string& key, int start, int end);
	void* cmdMode7(const char* cmd, const std::string& key, const std::string& start, const std::string& end, int count);
	void* cmdMode8(const char* cmd, const VEC_STRING& key);

	bool repMode1(const redisReply* rep);
	bool repMode2(const redisReply* rep);
	bool repMode3(const redisReply* rep, std::string& data);
	uint64_t repMode4(const redisReply* rep);
	bool repMode5(const redisReply* rep, VEC_STRING& vec);
	int64_t repMode6(const redisReply* rep);
	bool repMode7(const redisReply* rep, std::string& data);
	bool repMode8(const redisReply* rep, VEC_STRING& vec);

private:
	redisContext* _redisCtx;
	DB_REDIS_CONFIG_INFO _dbinfo;
};
