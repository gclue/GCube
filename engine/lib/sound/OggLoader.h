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
	ogg_int64_t size;			//!< バッファサイズ
	ogg_int64_t offset;			//!< オフセット
} MemoryStream;


/**
 * Oggファイル読み込みクラス.
 */
class OggLoader {
private:
	
	/**
	 * oggファイルをデコードします.
	 * @param vf oggファイル
	 * @param buf バッファ
	 * @param length バッファサイズ
	 * @return デコードサイズ
	 */
	size_t decode(OggVorbis_File* vf, char* buf, int length);
	
public:
	SoundData *load(const char *fname);
	
	/**
	 * 指定されたサウンドデータをOpenALに設定します.
	 * @param data サウンドデータ
	 * @return true: 成功
	 */
	SoundData *load(std::vector<char> *data);

};

#endif
