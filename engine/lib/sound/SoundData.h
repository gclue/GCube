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
class SoundData {
public:
	std::string filePath;	//!< ファイルパス.
	int dataSize;			//!< データサイズ
	void *data;				//!< データ
	ALenum fileType;		//!< ファイルタイプ
	ALsizei sampleRate;		//!< サンプリングレート
	
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
