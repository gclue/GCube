/*
 * The MIT License (MIT)
 * Copyright (c) 2012 GClue, inc.
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

#ifndef CoinPusher_main_h
#define CoinPusher_main_h

class ApplicationController;

/**
 * メインクラス.
 */
class Main {
private:
	ApplicationController *controller;
	
public:
	/**
	 * コンストラクタ.
	 */
	Main(ApplicationController *controller) {this->controller=controller;};
	
	/**
	 * デストラクタ.
	 */
	virtual ~Main() {};
	
	/**
	 * アプリケーション初期化処理.
	 */
	void initApplicationController();
	
	/**
	 * アプリ停止を通知します.
	 */
	void onPause();
	
	/**
	 * アプリ再開を通知します.
	 */
	void onResume();
	
	/**
	 * ゲームイベントを受け取るメソッド.
	 * @param type イベントタイプ
	 * @param param1 イベントパラメータ
	 * @param param2 イベントパラメータ
	 * @param param3 イベントパラメータ
	 * @param param4 イベントパラメータ
	 * @param param5 イベントパラメータ
	 * @return イベントを処理したらtrue,各シーンへイベントを渡す場合はfalse
	 */
	bool onGameEvent(int type, int param1, long param2, double param3, int param4, const char *param5);
	
	
	/**
	 * ツイッターイベントを受け取ります.
	 * @param[in] type イベントタイプ.
	 * @param[in] param1 パラメータ.
	 */
	bool onTwitterEvent(int type, int param1);
	
	/**
	 * ゲームセンターイベントのレスポンスを受け取ります.
	 * @param[in] type イベントタイプ.
	 * @param[in] param1 パラメータ1.
	 * @param[in] param2 パラメータ2.
	 */
	bool onGameCenterEvent(int type, int param1, int param2);
	
	/**
	 * デバッグコマンドを受け取るメソッド.
	 * @param command コマンド
	 * @param param パラメータ
	 * @return イベントを処理したらtrue,各シーンへイベントを渡す場合はfalse
	 */
	bool onDebugCommand(const char *command, int param);
	
	/**
	 * データ保存.
	 */
	void saveData();
};



#endif
