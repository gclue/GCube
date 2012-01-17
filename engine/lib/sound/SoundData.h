//
//  SoundData.h
//

#ifndef CoinPusher_SoundData_h
#define CoinPusher_SoundData_h

#ifdef __ANDROID__
#include <AL/al.h>
#include <AL/alc.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include <string>

/**
 * サウンドデータクラス.
 */
struct SoundData {
	// ファイルパス
	std::string filePath;
	// データサイズ
	int dataSize;
	// データ
	void *data;
	// ファイルタイプ
	ALenum fileType;
	// サンプリングレート
	ALsizei sampleRate;
	
	/**
	 * コンストラクタ.
	 */
	SoundData() {
		data=NULL;
	};
	
	/**
	 * デストラクタ.
	 */
	~SoundData() {
		if (data) free(data);
	};
};


#endif
