//
//  main.h
//

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
