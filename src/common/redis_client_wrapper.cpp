#include "redis_client_wrapper.h"
#include <assert.h>
#include <memory>
#include <sstream>
#include <strings.h>

CRedsiClientUnit::~CRedsiClientUnit()
{
	if (_redisCtx != nullptr)
		redisFree(_redisCtx);
}

CRedsiClientUnit::CRedsiClientUnit()
{
}

bool CRedsiClientUnit::Init(const DB_REDIS_CONFIG_INFO& st)
{
	_dbinfo = st;

	_redisCtx = redisConnect(_dbinfo.ip.c_str(), _dbinfo.port);
	assert(_redisCtx != nullptr);

	if (_redisCtx == nullptr)
		return false;
	
	if (_dbinfo.passwd.size() > 0)
	{
		auto rep = cmdMode1("AUTH", _dbinfo.passwd);
		assert(rep != nullptr);
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		if (!repMode1(ptr.get()))
			return false;
	}
	
	if (_dbinfo.dbindex > 0)
	{
		std::stringstream ss;
		ss << _dbinfo.dbindex;
		auto rep = cmdMode1("SELECT", ss.str());
		assert(rep != nullptr);
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		if (!repMode1(ptr.get()))
			return false;
	}

	return true;
}

bool CRedsiClientUnit::repMode1(const redisReply* rep)
{
	if (rep->type == REDIS_REPLY_STATUS && rep->len > 0)
		return strncasecmp(rep->str, "OK", rep->len) == 0;
	else
		return false;
}

bool CRedsiClientUnit::repMode2(const redisReply* rep)
{
	if (rep->type == REDIS_REPLY_INTEGER)
		return rep->integer == 1;
	else
		return false;
}

bool CRedsiClientUnit::repMode3(const redisReply* rep, std::string& data)
{
	if (rep->type == REDIS_REPLY_STRING && rep->len > 0)
	{
		data.assign(rep->str, rep->len);
		return true;
	}
	else if (rep->type == REDIS_REPLY_NIL)
		return true;
	else
		return false;
}

bool CRedsiClientUnit::repMode7(const redisReply* rep, std::string& data)
{
	if (rep->type == REDIS_REPLY_STRING)
	{
		if (rep->len > 0)
			data.assign(rep->str, rep->len);
		return true;
	}
	else if (rep->type == REDIS_REPLY_NIL)
		return true;
	else
		return false;
}

uint64_t CRedsiClientUnit::repMode4(const redisReply* rep)
{
	if (rep->type == REDIS_REPLY_INTEGER)
		return rep->integer;
	else
		return 0;
}

bool CRedsiClientUnit::repMode5(const redisReply* rep, VEC_STRING& vec)
{
	if (rep->type == REDIS_REPLY_ARRAY)
	{
		if (rep->elements > 0)
			vec.resize(rep->elements);
		
		for (size_t i = 0; i < rep->elements; i++)
		{
			redisReply* r = rep->element[i];
			if (r->type == REDIS_REPLY_STRING)
			{
				vec[i] = (r->len > 0 ? std::string(r->str, r->len) : std::string(""));
			}
		}
		return true;
	}
	else
		return false;
}

bool CRedsiClientUnit::repMode8(const redisReply* rep, VEC_STRING& vec)
{
	if (rep->type == REDIS_REPLY_ARRAY)
	{
		if (rep->elements > 0)
			vec.resize(rep->elements);

		for (size_t i = 0; i < rep->elements; i++)
		{
			redisReply* r = rep->element[i];
			if (r->type == REDIS_REPLY_STRING)
			{
				vec[i] = (r->len > 0 ? std::string(r->str, r->len) : std::string(""));
			}
			else if (r->type == REDIS_REPLY_NIL)
				vec[i] = std::string("");
		}
		return true;
	}
	else
		return false;
}

int64_t CRedsiClientUnit::repMode6(const redisReply* rep)
{
	if (rep->type == REDIS_REPLY_INTEGER)
		return rep->integer;
	else
		return -1;
}


void* CRedsiClientUnit::cmdMode1(const char* cmd, const std::string& key)
{
	return redisCommand(_redisCtx, "%s %b", cmd, key.c_str(), key.size());
}

void* CRedsiClientUnit::cmdMode2(const char* cmd, const std::string& key, const std::string& data)
{
	return redisCommand(_redisCtx, "%s %b %b", cmd, key.c_str(), key.size(), data.c_str(), data.size());
}

void* CRedsiClientUnit::cmdMode3(const char* cmd, const std::string& key, const std::string& field, const std::string& data)
{
	return redisCommand(_redisCtx, "%s %b %b %b", cmd, key.c_str(), key.size(), field.c_str(), field.size(), data.c_str(), data.size());
}

void* CRedsiClientUnit::cmdMode4(const char* cmd, const std::string& key, int start, int end)
{
	return redisCommand(_redisCtx, "%s %b %d %d", cmd, key.c_str(), key.size(), start, end);
}

void* CRedsiClientUnit::cmdMode5(const char* cmd, const std::string& key, uint32_t sec, const std::string& data)
{
	return redisCommand(_redisCtx, "%s %b %u %b", cmd, key.c_str(), key.size(), sec, data.c_str(), data.size());
}

void* CRedsiClientUnit::cmdMode6(const char* cmd, const std::string& key, int start, int end)
{
	return redisCommand(_redisCtx, "%s %b %d %d WITHSCORES", cmd, key.c_str(), key.size(), start, end);
}

void* CRedsiClientUnit::cmdMode7(const char* cmd, const std::string& key, const std::string& minscore, const std::string& maxscore, int count)
{
	return redisCommand(_redisCtx, "%s %b %s %s WITHSCORES LIMIT 0 %d", cmd, key.c_str(), key.size(), minscore.c_str(), maxscore.c_str(), count);
}

void* CRedsiClientUnit::cmdMode8(const char* cmd, const VEC_STRING& key)
{
	std::string strcmd = cmd;
	for (auto& it : key)
	{
		strcmd += " ";
		strcmd += it;
	}

	return redisCommand(_redisCtx, strcmd.c_str());
}

bool CRedsiClientUnit::set(const std::string& key, const std::string& data)
{
	auto rep = cmdMode2("SET", key, data);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode1(ptr.get());
	}
	return false;
}

bool CRedsiClientUnit::setnx(const std::string& key, const std::string& data)
{
	auto rep = cmdMode2("SETNX", key, data);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode2(ptr.get());
	}
	return false;
}

bool CRedsiClientUnit::get(const std::string& key, std::string& data)
{
	auto rep = cmdMode1("GET", key);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode3(ptr.get(), data);
	}
	return false;
}

bool CRedsiClientUnit::get2(const std::string& key, std::string& data)
{
	auto rep = cmdMode1("GET", key);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode7(ptr.get(), data);
	}
	return false;
}

uint64_t CRedsiClientUnit::incr(const std::string& key)
{
	auto rep = cmdMode1("INCR", key);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode4(ptr.get());
	}
	return 0;
}

bool CRedsiClientUnit::exists(const std::string& key)
{
	auto rep = cmdMode1("EXISTS", key);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode2(ptr.get());
	}
	return true;

}

bool CRedsiClientUnit::hgetall(const std::string& key, VEC_HASH_DATA& data)
{
	auto rep = cmdMode1("HGETALL", key);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		VEC_STRING vec;
		auto ret = repMode5(ptr.get(), vec);
		for (size_t i = 0; i < vec.size(); ++i)
		{
			HASH_UINT st;
			if ((i % 2) == 0)
			{
				st.field = vec[i];
				++i;
				st.data = vec[i];
				data.push_back(st);
			}
		}
		return ret;
	}
	return false;
}

bool CRedsiClientUnit::hsetnx(const std::string& key, const std::string& field, const std::string& data)
{
	void* rep = cmdMode3("HSETNX", key, field, data);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode2(ptr.get());
	}
	return false;
}

bool CRedsiClientUnit::hset(const std::string& key, const std::string& field, const std::string& data)
{
	void* rep = cmdMode3("HSET", key, field, data);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode2(ptr.get());
	}
	return false;
}

bool CRedsiClientUnit::hget(const std::string& key, const std::string& field, std::string& data)
{
	void* rep = cmdMode2("HGET", key, field);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode3(ptr.get(), data);
	}
	return false;
}

bool CRedsiClientUnit::rpush(const std::string& key, const std::string& data)
{
	void* rep = cmdMode2("RPUSH", key, data);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode4(ptr.get()) > 0;
	}
	return false;
}

bool CRedsiClientUnit::lrange(const std::string& key, int start, int end, VEC_STRING& vec)
{
	void* rep = cmdMode4("LRANGE", key, start, end);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode5(ptr.get(), vec);
	}
	return false;
}

bool CRedsiClientUnit::ltrim(const std::string& key, int start, int end)
{
	void* rep = cmdMode4("LTRIM", key, start, end);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode1(ptr.get());
	}
	return false;
}

bool CRedsiClientUnit::setex(const std::string& key, const std::string& data, uint32_t sec)
{
	void* rep = cmdMode5("SETEX", key, sec, data);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode1(ptr.get());
	}
	return false;
}

bool CRedsiClientUnit::zadd(const std::string& key, uint32_t score, const std::string& member)
{
	void* rep = cmdMode5("ZADD", key, score, member);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		auto ret = repMode4(ptr.get());
		return ret == 0 || ret == 1;
	}
	return false;
}

bool CRedsiClientUnit::zrem(const std::string& key, const std::string& member)
{
	void* rep = cmdMode2("ZREM", key, member);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		auto ret = repMode4(ptr.get());
		return ret == 1;
	}
	return false;
}

void CRedsiClientUnit::zincrby(const std::string& key, uint32_t score, const std::string& member)
{
	void* rep = cmdMode5("ZINCRBY", key, score, member);
	if (rep != nullptr)
	{
		freeReplyObject(rep);
	}
}

int CRedsiClientUnit::zrevrank(const std::string& key, const std::string& member)
{
	void* rep = cmdMode2("ZREVRANK", key, member);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode6(ptr.get());
	}
	return -1;
}

bool CRedsiClientUnit::zrevrange(const std::string& key, int start, int end, VEC_STRING& vec)
{
	void* rep = cmdMode6("ZREVRANGE", key, start, end);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode5(ptr.get(), vec);
	}
	return vec.size() > 0;
}

uint32_t CRedsiClientUnit::zscore(const std::string& key, const std::string& member)
{
	void* rep = cmdMode2("ZSCORE", key, member);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		std::string data;
		if (repMode3(ptr.get(), data))
			return atoi(data.c_str());
		else
			return 0;
	}
	return 0;
}

bool CRedsiClientUnit::del(const std::string& key)
{
	void* rep = cmdMode1("DEL", key);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode2(ptr.get());
	}
	return false;
}

bool CRedsiClientUnit::zrangebyscore(const std::string& key, const std::string& minscore, const std::string& maxscore, int count, VEC_STRING& vec)
{
	void* rep = cmdMode7("ZRANGEBYSCORE", key, minscore, maxscore, count);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode5(ptr.get(), vec);
	}
	return vec.size() > 0;
}

bool CRedsiClientUnit::sadd(const std::string& key, const std::string& data)
{
	void* rep = cmdMode2("SADD", key, data);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode2(ptr.get());
	}
	return false;
}

bool CRedsiClientUnit::srem(const std::string& key, const std::string& data)
{
	void* rep = cmdMode2("SREM", key, data);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		return repMode2(ptr.get());
	}
	return false;
}

void CRedsiClientUnit::smembers(const std::string& key, VEC_STRING& vecdata)
{
	void* rep = cmdMode1("SMEMBERS", key);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		repMode5(ptr.get(), vecdata);
	}
}

void CRedsiClientUnit::keys(const std::string& key, VEC_STRING& vecdata)
{
	void* rep = cmdMode1("KEYS", key);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		repMode5(ptr.get(), vecdata);
	}
}

void CRedsiClientUnit::mget(const VEC_STRING& key, VEC_STRING& data)
{
	void* rep = cmdMode8("MGET", key);
	if (rep != nullptr)
	{
		std::shared_ptr<redisReply> ptr(reinterpret_cast<redisReply*>(rep), freeReplyObject);
		repMode8(ptr.get(), data);
	}
}
