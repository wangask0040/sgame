#pragma once
#include <event.h>
#include <evhttp.h>
#include <string>
#include <map>

using MAP_URL_PARAM = std::map<std::string, std::string>;

class IHttpServer
{
public:
	IHttpServer();
	virtual ~IHttpServer();
	void start(const std::string& ip, int port);
	static void OnRecvHttpReq(evhttp_request* req, void* ctx);
	virtual void HandleHttpReq(evhttp_request* req) = 0;
	static void GetBodyData(evhttp_request* req, std::string& input);
	static void QuerySplit(const char* query, MAP_URL_PARAM& ret);
	static void ResponseBodyData(evhttp_request* req, const std::string& data);
	static const char* GetUrlPath(evhttp_request* req);
	static const char* GetUrlQuery(evhttp_request* req);

protected:
	event_base* _base = nullptr;
	evhttp* _http = nullptr;
private:

};