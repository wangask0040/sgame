#pragma once
#include "common/http_server.h"
#include "common/singleton.h"

class CGmSrv
	: public IHttpServer
{
public:
	CGmSrv() {}
	~CGmSrv() {}
	bool Init(int srvtype, int srvidx);
	void HandleHttpReq(evhttp_request* req) override;
	int SrvType() const { return _srv_type; }
	int SrvIdx() const { return _srv_idx; }

protected:
	int _srv_type;
	int _srv_idx;
};

using CGmSrvSgt = Singleton<CGmSrv>;