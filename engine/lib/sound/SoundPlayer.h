//
//  SoundPlayer.h
//

#ifndef CoinPusher_SoundPlayer_h
#define CoinPusher_SoundPlayer_h

#include <map>
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

	/** デストラクタ. */
	virtual ~SoundPlayer();

	// OpenALのコンテキスト
	ALCdevice *device;
	ALCcontext *context;
	std::map<ALuint, ALuint> sounds;

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
	int loadSound(SoundData *data);
	
	/**
	 * 登録ID指定でサウンドを削除します.
	 * @param[in] source 登録ID
	 */
	void removeSound(int source);
	
	/**
	 * すべてのサウンドを削除します.
	 */
	void resetAllSounds();
	
	/**
	 * 登録ID指定でサウンドを再生します.
	 * @param[in] source 登録ID
	 * @param[in] loop ループ再生
	 */
	void play(int source, bool loop);
	
	/**
	 * 登録ID指定でサウンドを停止します.
	 * @param[in] source 登録ID
	 */
	void stop(int source);
	
	/**
	 * 登録ID指定で音量を調整します.
	 * @param[in] source 登録ID
	 * @param[in] volume 音量(0〜 1.0)
	 */
	void volume(int source, float volume);

	/**
	 * 登録ID指定で再生中か確認します.
	 * @param[in] source 登録ID
	 * @return true 再生中
	 */
	bool isPlaying(int source);

	/**
	 * SoundPlayerを破棄します.
	 */
	static void dispose();
};


#endif
