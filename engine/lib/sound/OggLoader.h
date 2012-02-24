//
//  OggLoader.h
//

#ifndef CoinPusher_OggLoader_h
#define CoinPusher_OggLoader_h

#include "SoundData.h"
#include <vector>
#include "ogg/ogg.h"
#include "vorbis/vorbisfile.h"

/**
 * @struct 簡易メモリストリーム.
 */
typedef struct _MemoryStream {
	unsigned char* buffer;	//!< バッファ
	ogg_int64_t size;		//!< バッファサイズ
	ogg_int64_t offset;		//!< オフセット
} MemoryStream;


/**
 * Oggファイル読み込みクラス.
 */
class OggLoader {
private:
	/**
	 * Wavファイルをロードします.
	 * @param[in] data サウンドデータ
	 * @return SoundData
	 */
	SoundData* loadWav(std::vector<char> *data);

	/**
	 * Oggファイルをロードします.
	 * @param[in] data サウンドデータ
	 * @return SoundData
	 */
	SoundData* loadOgg(std::vector<char> *data);

	/**
	 * oggファイルをデコードします.
	 * @param vf oggファイル
	 * @param buf バッファ
	 * @param length バッファサイズ
	 * @return デコードサイズ
	 */
	size_t decode(OggVorbis_File* vf, char* buf, int length);
	
public:
	/**
	 * 指定されたファイルのサウンドデータを読み込みます.
	 * assetsに格納されているファイルを読み込みます.
	 * @param[in] fname ファイル名
	 */
	SoundData *load(const char *fname);
	
	/**
	 * 指定されたサウンドデータをOpenALに設定します.
	 * @param data サウンドデータ
	 * @return true: 成功
	 */
	SoundData *load(std::vector<char> *data);
};

#endif
