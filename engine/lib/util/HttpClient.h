/*
 * HttpClient.h
 *
 *  Created on: 2011/11/11
 *      Author: GClue, Inc.
 */
#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

#include <string>
#include <map>

#include "APIGlue.h"

class HttpRequestBase {
private:
	std::string url;							//!< リクエスト先のURL
	std::map<std::string, std::string> headers;	//!< ヘッダー一覧
	int userID;									//!< ユーザ識別用のID
	void* userObj;								//!< ユーザ識別用オブジェクト

public:
	HttpRequestBase(std::string u) : url(u) {
		userID = 0;
		userObj = NULL;
	}
	virtual ~HttpRequestBase() {}

	virtual const std::string& getUrl() {
		return url;
	}

	virtual int getUserID() {
		return userID;
	}

	virtual void setUserID(int id) {
		userID = id;
	}

	virtual void* getUserObj() {
		return userObj;
	}

	virtual void setUserObj(void* obj) {
		userObj = obj;
	}

	virtual const std::map<std::string, std::string>& getHeaders() {
		return headers;
	}

	virtual void addHeader(std::string key, std::string value) {
		headers[key] = value;
	}

	virtual const std::string getHeader(std::string key) {
		std::map<std::string, std::string>::iterator ret = headers.find(key);
		if (ret != headers.end()) {
			return ret->second;
		}
		return NULL;
	}
};

class HttpGet : public HttpRequestBase {
public:
	HttpGet(std::string u) : HttpRequestBase(u) {}
	virtual ~HttpGet() {}
};

class HttpPost : public HttpRequestBase {
private:
	std::string body;
public:
	HttpPost(std::string u) : HttpRequestBase(u) {}
	virtual ~HttpPost() {}

	virtual void addBody(const char *data) {
		body.append(data);
	}

	virtual void setBody(const char *data) {
		body.clear();
		body.append(data);
	}

	virtual const std::string& getBody() {
		return body;
	}
};

/**
 * Http通信のレスポンスを格納するためのクラス.
 */
class HttpResponse {
private:
	std::string url;							//!< リクエスト先のURL
	std::map<std::string, std::string> headers;	//!< ヘッダー一覧
	std::string data;							//!< 返却されたデータ
	int statusCode;								//!< ステータスコード

public:
	HttpResponse() {}
	virtual ~HttpResponse() {}

	virtual void setStatusCode(int statusCode) {
		this->statusCode = statusCode;
	}

	virtual int getStatusCode() {
		return statusCode;
	}

	virtual void addData(const char *data) {
		this->data.append(data);
	}

	virtual void addData(const char *data, int size) {
		this->data.append(data, size);
	}

	virtual void setData(const char *data) {
		this->data.clear();
		this->data.append(data);
	}

	virtual void setData(const char *data, int size) {
		this->data.clear();
		this->data.append(data, size);
	}

	virtual const std::string& getData() {
		return data;
	}

	virtual void addHeader(std::string& key, std::string& value) {
		headers.insert(std::map<std::string, std::string>::value_type(key, value));
	}

	virtual std::string& getHeader(std::string& key) {
		return headers[key];
	}
};

/**
 * Http通信を非同期で行ったときのレスポンスを受けるリスナー.
 */
class IHttpClientListener {
public:
	virtual ~IHttpClientListener() {}

	/**
	 * Http通信のレスポンスを受け取ったときの処理.
	 * @param request リクエスト
	 * @param response レスポンス
	 */
	virtual void onResponse(HttpRequestBase *request, HttpResponse *response) = 0;
};

/**
 * Http通信を行うためのシンプルなクライアント.
 *
 * @code
 * HttpGet get("http://www.hoge.com");
 * get.addHeader("HOGE", "HOGE");
 *
 * HttpClient* client = new HttpClient();
 * HttpResponse* response = client->execute(get);
 * if (response) {
 * 	if (response.getStatusCode() == 200) {
 * 		// 通信成功
 * 	} else {
 * 		// 通信失敗
 * 	}
 * }
 * @endcode
 */
class HttpClient {
public:
	enum {
		HTTP_OK = 0,	//!< HTTP通信のリクエストに成功
		HTTP_FAILED,	//!< HTTP通信のリクエストに失敗
	};

	HttpClient();
	virtual ~HttpClient();

	/**
	 * 同期的にHTTP通信を行います.
	 * @param request HTTPリクエスト
	 * @return HTTP通信の結果
	 */
	HttpResponse* execute(HttpGet& request);

	/**
	 * 同期的にHTTP通信を行います.
	 * @param request HTTPリクエスト
	 * @return HTTP通信の結果
	 */
	HttpResponse* execute(HttpPost& request);

	/**
	 * 非同期でHTTP通信を実行します.
	 *
	 * listenerのポインタは解放しないので、使用する先で不必要になった時点で解放してください。
	 * ただし、通知がすべて返った後に解放してください。
	 * 通知が発生する前にlistenerが解放されていた場合にはメモリへの不正アクセスが発生してしまいます。
	 *
	 * @param request HTTPリクエスト
	 * @param listner HTTP通信の結果を通知するリスナー
	 * @return 実行の正否
	 */
	int execute(HttpGet& request, IHttpClientListener *listner);

	/**
	 * 非同期でHTTP通信を実行します.
	 *
	 * listenerのポインタは解放しないので、使用する先で不必要になった時点で解放してください。
	 * ただし、通知がすべて返った後に解放してください。
	 * 通知が発生する前にlistenerが解放されていた場合にはメモリへの不正アクセスが発生してしまいます。
	 *
	 * @param request HTTPリクエスト
	 * @param listner HTTP通信の結果を通知するリスナー
	 * @return 実行の正否
	 */
	int execute(HttpPost& request, IHttpClientListener *listner);
};

#endif /* HTTPCLIENT_H_ */
