#pragma once
#include <event.h>
#include <string>
#include <event2/http.h>

class IHttpClient
{
public:
	IHttpClient(event_base* b);
	virtual ~IHttpClient();

	void Get(const std::string& url);
	void Post(const std::string& url, const std::string& data);
	virtual int OnResponse(const std::string& body) = 0;
	static void HttpRequstDone(struct evhttp_request *req, void *arg);

protected:
	event_base* Base() { return _base; }
	void Release();
	void Send(int cmd, const std::string& url, const std::string& body);

private:
	event_base* _base = nullptr;
	evhttp_connection* _conn = nullptr;
	unsigned int _time_out_sec = 20;
};