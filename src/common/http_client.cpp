#include "http_client.h"
#include <assert.h>
#include <event2/http_struct.h>

IHttpClient::IHttpClient(event_base* b)
{
	_base = b;
}

IHttpClient::~IHttpClient()
{
	Release();
}

void IHttpClient::Get(const std::string& url)
{
	std::string body;
	Send(EVHTTP_REQ_GET, url, body);
}

void IHttpClient::HttpRequstDone(struct evhttp_request *req, void *arg)
{
	IHttpClient* p = reinterpret_cast<IHttpClient*>(arg);
	if (req != nullptr)
	{
		auto evbuf = evhttp_request_get_input_buffer(req);
		auto len = evbuffer_get_length(evbuf);
		if (len > 0)
		{
			auto buf = new char[len];
			auto s = evbuffer_copyout(evbuf, buf, len);
			assert(static_cast<size_t>(s) == len);
			std::string tmp(buf, len);
			delete[] buf;

			p->OnResponse(tmp);
		}
	}
	else
		assert(req != nullptr);
}

void IHttpClient::Release()
{
	if (_conn != nullptr)
		evhttp_connection_free(_conn);
}

void IHttpClient::Send(int cmd, const std::string& url, const std::string& body)
{
	Release();

	auto url_encode_str = evhttp_encode_uri(url.c_str());
	auto uri = evhttp_uri_parse(url_encode_str);
	free(url_encode_str);

	auto host = evhttp_uri_get_host(uri);
	auto port = evhttp_uri_get_port(uri);
	if (port == -1)
		port = 80;

	_conn = evhttp_connection_base_new(_base, nullptr, host, port);
	assert(_conn != nullptr);

	//evhttp_connection_free_on_completion(conn);
	evhttp_connection_set_timeout(_conn, _time_out_sec);

	auto req = evhttp_request_new(HttpRequstDone, this);
	assert(req != nullptr);

	if (body.size() > 0)
	{
		auto evbuf = evhttp_request_get_output_buffer(req);
		assert(evbuf != nullptr);
		evbuffer_add(evbuf, body.c_str(), body.size());
	}

	evhttp_make_request(_conn, req, (evhttp_cmd_type)cmd, url.c_str());

	evhttp_uri_free(uri);
}

void IHttpClient::Post(const std::string& url, const std::string& data)
{
	Send(EVHTTP_REQ_POST, url, data);
}
