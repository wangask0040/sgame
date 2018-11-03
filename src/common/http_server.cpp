#include "http_server.h"
#include <assert.h>
#include <iostream>
#include "logging.h"
#include "stringex.h"

IHttpServer::IHttpServer()
{
	_base = event_base_new();
	_http = evhttp_new(_base);
}

IHttpServer::~IHttpServer()
{
	evhttp_free(_http);
	event_base_free(_base);
}

void IHttpServer::start(const std::string& ip, int port)
{
	evhttp_set_gencb(_http, IHttpServer::OnRecvHttpReq, this);
	auto ret = evhttp_bind_socket(_http, ip.c_str(), static_cast<unsigned short>(port));
	if (ret != 0)
	{
		assert(false);
		return;
	}

	NLOGINFO << "http server start!" << ip << ":" << port;
	event_base_dispatch(_base);
}

void IHttpServer::GetBodyData(evhttp_request* req, std::string& input)
{
	auto evbuf = evhttp_request_get_input_buffer(req);
	auto len = evbuffer_get_length(evbuf);
	if (len > 0)
	{
		auto buf = new char[len];
		if ((size_t)evbuffer_copyout(evbuf, buf, len) == len)
			input.append(buf, len);
		delete[] buf;
	}
}

void IHttpServer::OnRecvHttpReq(evhttp_request* req, void* ctx)
{
	//auto method = evhttp_request_get_command(req);
	IHttpServer* p = reinterpret_cast<IHttpServer*>(ctx);
	p->HandleHttpReq(req);
}

void IHttpServer::QuerySplit(const char* query, MAP_URL_PARAM& ret)
{
	std::vector<std::string> vec;
	split(query, URL_PARAM_SPLIT, vec);
	
	if (vec.size() == 0)
		vec.push_back(query);

	for (auto& it : vec)
	{
		std::vector<std::string> tmp;
		split(it, URL_PARAM_VALUE_SPLIT, tmp);
		if (tmp.size() > 1)
			ret[tmp[0]] = tmp[1];
		else
			std::cout << "query fomat err. query:" << query;
	}	
}

void IHttpServer::ResponseBodyData(evhttp_request* req, const std::string& data)
{
	auto evbuf = evhttp_request_get_output_buffer(req);
	evbuffer_add(evbuf, data.c_str(), data.size());
	evhttp_send_reply(req, 200, "OK", nullptr);
}

const char* IHttpServer::GetUrlPath(evhttp_request* req)
{
	auto path = evhttp_uri_get_path(req->uri_elems);
	if (path != nullptr)
		NLOGDEBUG << path;

	return path;
}

const char* IHttpServer::GetUrlQuery(evhttp_request* req)
{
	// 获取查询字符串, 并且对特殊字符进行解码
	auto query = evhttp_uri_get_query(req->uri_elems);
	if (query != nullptr)
	{
		query = evhttp_decode_uri(query);
		NLOGDEBUG << query;
	}
	return query;
}