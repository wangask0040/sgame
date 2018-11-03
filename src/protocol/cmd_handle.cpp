#include "cmd_handle.h"
#include "common/logging.h"

bool CCmdHanleMgr::Add(int cmd, ICmdHandleUnit* ptr)
{
	auto it = _handle.find(cmd);
	if (it == _handle.end())
	{
		_handle[cmd] = ptr;
		return true;
	}
	else
	{
		NLOGERROR << "cmd:" << cmd;
		assert(false);
		return false;
	}
}

void CCmdHanleMgr::ReleaseAll()
{
	for (auto& it : _handle)
	{
		delete it.second;
		_handle.erase(it.first);
	}
}

void CCmdHanleMgr::Handle(const CMD_HANDLE_REQ& req, CMD_HANDLE_REPLY& reply)
{
	auto& info = req.info;
	auto cmd = info.head().cmd();
	auto it = _handle.find(cmd);
	if (it != _handle.end())
	{
		it->second->Enter(req, reply);
	}
	else
	{
		NLOGERROR << "not find cmd:" << cmd;
	}
}
