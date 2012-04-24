/*
 * The MIT License (MIT)
 * Copyright (c) 2011 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * APIGlue.h
 *
 *  Created on: 2011/09/16
 *      Author: GClue, Inc.
 */
#ifndef APIGLUE_H_
#define APIGLUE_H_

#include <vector>
#include <string>
#include <map>

/**
 * サウンドモード.
 */
enum SoundMode {
	SoundMode_CreateBGM = 0,	//!< BGM作成
	SoundMode_DisposeBGM,		//!< BGM削除
	SoundMode_PlayBGM ,			//!< BGM再生
	SoundMode_StopBGM,			//!< BGM停止
	SoundMode_RepeatBGM,		//!< BGMの連続停止（同じ名前のファイルの場合には、続けて再生します
	SoundMode_PauseBGM,			//!< BGMの一時停止
	SoundMode_ResumeBGM,		//!< BGMの再開
	SoundMode_PrepareSE,		//!< SE登録
	SoundMode_DisposeSE,		//!< SE削除
	SoundMode_PlaySE,			//!< SE再生
	SoundMode_StopSE,			//!< SE停止
	SoundMode_StopAllSE,		//!< すべてのSE停止
	SoundMode_PauseSE,			//!< SEの一時停止
	SoundMode_ResumeSE,			//!< SEの再生再開
};

/**
 * Java側の処理の結果.
 */
enum {
	JAVA_SUCCESS = -1,			//!< 成功
	JAVA_UNKNOWN_ERROR = 1,		//!< 原因不明のエラー
	JAVA_OFFLINE = 2,			//!< オフライン
	JAVA_SERVER_ERROR = 4,		//!< サーバーエラー
	JAVA_BAD_REQUEST = 8,		//!< リクエストが不正
	JAVA_AUTH_CANCEL = 16,		//!< プレイヤーによる認証キャンセル
};

/**
 * WebViewのイベントID.
 */
enum {
	WebViewEvent_AddView = 0,
	WebViewEvent_CloseView,
	WebViewEvent_RemoveView,
	WebViewEvent_ReloadPage,
	WebViewEvent_LoadURL,
	WebViewEvent_ResizeView,
	WebViewEvent_SetPosition,
};

/**
 * TwitterのイベントID.
 */
enum {
	TwitterEvent_Post = 0,
	TwitterEvent_Authenticate,
};

/**
 * AdMobのイベントID.
 */
enum {
	AdMobEvent_Create = 0,
	AdMobEvent_Visible,
	AdMobEvent_Invisible,
};

/**
 * AdMobの広告位置のID.
 */
enum {
	ADMOB_POS_UP_CENTER = 0,
	ADMOB_POS_DOWN_CENTER,
	ADMOB_POS_CENTER,
	ADMOB_POS_LEFT_UP_CORNER,
	ADMOB_POS_LEFT_DOWN_CORNER,
	ADMOB_POS_RIGHT_UP_CORNER,
	ADMOB_POS_RIGHT_DOWN_CORNER
};


/**
 * ゲームセンターイベントID.
 */
enum {
	GameCenterEvent_Login = 0,
	GameCenterEvent_Send_Score_Long,
	GameCenterEvent_Send_Score_Integer,
	GameCenterEvent_Send_Score_Double,
	GameCenterEvent_Open_LeaderBoard,
	GameCenterEvent_Close_LeaderBoard,
};

class Figure;
class Texture;
class TextTexture;
class PackerTexture;
class ApplicationController;
class HttpResponse;

class IHttpRquestListener {
public:
	virtual ~IHttpRquestListener() {}
	virtual void onResponse(HttpResponse *response) = 0;
};


extern "C" {

/**
 * 文字列を描画したテクスチャを取得します.
 * もし文字列の描画が行われていなかった場合にはNULLを返却します.
 *
 * この関数を呼び出す前にJNIDrawTextで実行された文字列を描画したテクスチャを作成します。
 * この関数が呼ばれるとJNIDrawTextで登録された文字列はすべてJava上から削除されてしまうので
 * 再度、この関数を呼び出すときはJNIDrawTextを行ってください。
 *
 * また、PackerTextureに登録されているテスチャIDはJNIDrawTextで登録した順番になっています。
 *
 * @return Textureクラス
 */
PackerTexture* GCGetTextTexture();

/**
 * 文字列で描画したテクスチャを再読み込みをします。
 * GCDrawTextをコールしている途中には使わないでください。
 */
bool GCReloadTextTexture(TextTexture *texture);

/**
 * 指定された文字列をテクスチャに書き込みます.
 * @param[in] text 文字列
 * @param[in] fontSize フォントサイズ
 * @param[in] r 赤色成分
 * @param[in] g 緑色成分
 * @param[in] b 青色成分
 * @return テクスチャデータ
 */
void GCDrawText(const char *text, float fontSize, float r, float g, float b);

/**
 * SE,BGM再生
 * mode: 0:BGM再生
 *       1:BGM停止
 *       2:SE登録
 *       3:SE再生
 * @param[in] fname ファイル名
 * @param[in] mode モード
 */
void GCSoundEvent(const char *fileName, int mode);

/**
 * Assetの中を読み込みます.
 * @param[in] fileName ファイル名
 * @return charのvectorオブジェクト
 */
std::vector<char>* GCLoadAsset(const char *fileName);

/**
 * イベントを配送します.
 * @param[in] type イベントタイプ
 * @param[in] param1 イベントパラメータ
 * @param[in] param2 イベントパラメータ
 * @param[in] param3 イベントパラメータ
 * @param[in] param4 イベントパラメータ
 * @param[in] param5 イベントパラメータ
 */
void GCSendGameEvent(int type, int param1, long param2, double param3, int param4, const char *param5);

/**
 * テクスチャを読み込みます.
 * @param[in] fname ファイル名
 * @return テクスチャオブジェクト
 */
bool GCLoadTexture(Texture *texture, const char *fname);

/**
 * 保存領域へのパスを取得します.
 * @return パス
 */
const char* GCGetStoragePath(const char* fileName=NULL);

/**
 * 非同期にHTTP通信を行います.
 * @param url 通信先のURL
 * @param headers ヘッダー
 * @param body ボディ
 * @param callback HTTP通信を行った結果を返すリスナー
 * @return 0以下の場合には通信失敗
 */
int GCHttpRequestAsync(std::string url, std::map<std::string, std::string> headers, std::string body, IHttpRquestListener *callback);

/**
 * HTTP通信を行います.
 * @param url 通信先のURL
 * @param headers ヘッダー
 * @param body ボディ
 * @return HTTP通信の結果
 */
HttpResponse* GCHttpRequest(std::string url, std::map<std::string, std::string> headers, std::string body);

	
/**
 * WebViewのイベントをNativeに送ります.
 * @param[in] type イベントタイプ.
 * @param[in] viewId WebViewのビューID(ユーザーID).
 * @param[in] param1 パラメータ.
 * @param[in] param2 パラメータ.
 * @param[in] param3 パラメータ.
 * @param[in] param4 パラメータ.
 * @param[in] param5 パラメータ.
 */
void GCSendWebViewEvent(int type, int viewId, double param1, double param2, double param3, double param4, const char *param5);

/**
 * ツイッターのイベントをNativeに投げます.
 * @param[in] type ツイッターイベントタイプ.
 * @param[in] text 投稿文.
 */
void GCSendTwitterEvent(int type, const char* text);
	
/**
 * ゲームセンターのイベントをNativeに投げます(iPhone only).
 * <br>
 * 設定したスコアの型に合わせてスコアを設定してください。
 * @param[in] type イベントタイプ.
 * @param[in] lScore ロング型のスコア.
 * @param[in] iScore int型のスコア.
 * @param[in] dScore double型のスコア.
 */
void GCSendGameCenterEvent(int type, long long lScore, int iScore, double dScore);

/**
 * AdMobのイベントをNativeに送信します
 * @param[in] type イベントタイプ.
 */
void GCSendAdMobEvent(int type, int pos, const char* unitid);
}

#endif /* APIGLUE_H_ */
