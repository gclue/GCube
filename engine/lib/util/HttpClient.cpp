/*
 * HttpClient.cpp
 *
 *  Created on: 2011/11/11
 *      Author: GClue, Inc.
 */
#include "HttpClient.h"
#include "Log.h"

class HttpRequestListenerImpl : public IHttpRquestListener {
private:
	HttpRequestBase request;
	IHttpClientListener *listener;
public:
	HttpRequestListenerImpl(HttpRequestBase& r, IHttpClientListener* l);
	~HttpRequestListenerImpl();

	virtual void onResponse(HttpResponse *response);
};

HttpRequestListenerImpl::HttpRequestListenerImpl(HttpRequestBase& r, IHttpClientListener *l) : request(r) {
	listener = l;
}

HttpRequestListenerImpl::~HttpRequestListenerImpl() {
	listener = NULL;
}

void HttpRequestListenerImpl::onResponse(HttpResponse *response) {
	if (listener) {
		listener->onResponse(&request, response);
	}
	// レスポンスが返ってきたので、自分自身を削除する
	delete this;
}


HttpClient::HttpClient() {
}

HttpClient::~HttpClient() {
}

HttpResponse* HttpClient::execute(HttpGet& request) {
	// GETのリクエストなのでbodyは空にしておく
	std::string body;
	// リクエスト
	return GCHttpRequest(request.getUrl(), request.getHeaders(), body);
}

HttpResponse* HttpClient::execute(HttpPost& request) {
	return GCHttpRequest(request.getUrl(), request.getHeaders(), request.getBody());
}

int HttpClient::execute(HttpGet& request, IHttpClientListener *listener) {
	// リスナーが設定されていないときには通信をさせない
	if (!listener) {
		return HTTP_FAILED;
	}

	HttpRequestListenerImpl *impl = new HttpRequestListenerImpl(request, listener);
	if (impl) {
		// GETのリクエストなのでbodyは空にしておく
		std::string body;
		// 実際のリクエストを行う
		int result = GCHttpRequestAsync(request.getUrl(), request.getHeaders(), body, impl);
		if (!result) {
			// リクエストに失敗したので、解放する
			delete impl;
		}
		return result;
	}
	return HTTP_FAILED;
}

int HttpClient::execute(HttpPost& request, IHttpClientListener *listener) {
	// リスナーが設定されていないときには通信をさせない
	if (!listener) {
		return HTTP_FAILED;
	}
	HttpRequestListenerImpl *impl = new HttpRequestListenerImpl(request, listener);
	if (impl) {
		// 実際のリクエストを行う
		int result = GCHttpRequestAsync(request.getUrl(), request.getHeaders(), request.getBody(), impl);
		if (!result) {
			// リクエストに失敗したので、解放する
			delete impl;
		}
		return result;
	}
	return HTTP_FAILED;
}

