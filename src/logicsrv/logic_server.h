#pragma once
#include "common/http_server.h"
class CLogicSrv
	: public IHttpServer
{
public:
	CLogicSrv();
	~CLogicSrv();
	bool Init() const;
	void HandleHttpReq(evhttp_request* req) override;
};

