#pragma once
#include "common/websocket_sever.h"

class CGateSrv
	: public IWebSocketSever
{
public:
	CGateSrv();
	~CGateSrv();
	bool Init(int srvtype, int srvidx);
	void OnWSRead(const std::string& data, bufferevent* bev) override;

protected:
	int _srv_type;
	int _srv_idx;
};

