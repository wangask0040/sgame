#pragma once
#include "msg_info.pb.h"
#include <unordered_map>
#include "common/singleton.h"
#include "common/struct_def.h"

struct CMD_HANDLE_REQ
{
	MSG_INNER_HEAD head;
	protocol::msg_info info;
};

struct CMD_HANDLE_REPLY
{
	std::string data;
};

class ICmdHandleUnit
{
public:
	ICmdHandleUnit() {}
	virtual ~ICmdHandleUnit() {}
	virtual int Enter(const CMD_HANDLE_REQ& req, CMD_HANDLE_REPLY& reply) = 0;

protected:
};

using MAP_CMD_HANDLE = std::unordered_map<int, ICmdHandleUnit*>;

class CCmdHanleMgr
{
public:
	CCmdHanleMgr() {}
	~CCmdHanleMgr() {}
	bool Add(int cmd, ICmdHandleUnit* ptr);
	void ReleaseAll();
	void Handle(const CMD_HANDLE_REQ& info, CMD_HANDLE_REPLY& reply);

protected:
	MAP_CMD_HANDLE _handle;
};

using CCmdHanleMgrSgt = Singleton<CCmdHanleMgr>;

