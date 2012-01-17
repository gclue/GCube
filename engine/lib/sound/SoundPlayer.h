//
//  SoundPlayer.h
//

#ifndef CoinPusher_SoundPlayer_h
#define CoinPusher_SoundPlayer_h

#include "SoundData.h"

/**
 * サウンド再生クラス（シングルトン）.
 */
class SoundPlayer {
private:
	// 唯一のインスタンス
	static SoundPlayer *instance;
    // 生成やコピーを禁止する
    SoundPlayer();
    SoundPlayer(const SoundPlayer& rhs);
    SoundPlayer& operator=(const SoundPlayer& rhs);
	// OpenALのコンテキスト
	ALCdevice *device;
	ALCcontext *context;
	
public:
	/**
	 * インスタンスを取得します.
	 * @return SoundPlayerオブジェクト
	 */
    static SoundPlayer *getInstance();
	
	/**
	 * SoundDataオブジェクト指定でサウンドを登録します.
	 * @param[in] data SoundData
	 * @return 登録ID
	 */
    static int loadSound(SoundData *data);
	
	/**
	 * 登録ID指定でサウンドを削除します.
	 * @param[in] source 登録ID
	 */
	static void removeSound(int source);
	
	/**
	 * すべてのサウンドを削除します.
	 */
	static void resetAllSounds();
	
	/**
	 * 登録ID指定でサウンドを再生します.
	 * @param[in] source 登録ID
	 */
	static void play(int source);
	
	/**
	 * 登録ID指定でサウンドを停止します.
	 * @param[in] source 登録ID
	 */
	static void stop(int source);
	
	/**
	 * 登録ID指定で音量を調整します.
	 * @param[in] source 登録ID
	 * @param[in] volume 音量
	 */
	static void volume(int source, float volume);

};


#endif
