//
//  OggLoader.cpp
//

#include "AssetManager.h"
#include "OggLoader.h"
#include "Log.h"
#include <stdlib.h>

/** Wavファイルのヘッダーを定義. */
typedef struct {
	char riff[4];					//!< 'RIFF'
	unsigned int riffSize;			//!< (ファイルのバイト数)-8
	char wave[4];					//!< 'WAVE'
	char fmt[4];					//!< 'fmt '
	unsigned int fmtSize;			//!< PCMWAVEFORMAT構造体のバイト数=常に16
	// PCMWAVEFORMAT構造
	unsigned short format;
	unsigned short channels;
	unsigned int samplesPerSec;
	unsigned int bytesPerSec;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char data[4];					//!< 'data'
	unsigned int dataSize;
} WAVHEADER;

static size_t ogg_read_func(void *ptr, size_t size, size_t nmemb, void *datasource) {
	MemoryStream* stream = (MemoryStream*) datasource;
	
	size_t length = size * nmemb;
	if (length > stream->size - stream->offset) {
		length = stream->size - stream->offset;
	}
	
	if (length > 0) {
		memcpy(ptr, stream->buffer + stream->offset, length);
		stream->offset += length;
	}
	return length;
}

static int ogg_seek_func(void *datasource, ogg_int64_t offset, int whence) {
	MemoryStream* stream = (MemoryStream*) datasource;
	
	switch (whence) {
		case SEEK_CUR:
			stream->offset += offset;
			break;
		case SEEK_END:
			stream->offset = stream->size + offset;
			break;
		case SEEK_SET:
			stream->offset = offset;
			break;
		default:
			LOGD("Failed to seek the ogg file - offset: %ld", offset);
			return -1;
	}
	
	if (stream->offset > stream->size) {
		stream->offset = stream->size;
		return -1;
	} else if (stream->offset < 0) {
		stream->offset = 0;
		return -1;
	}
	return 0;
}

static long ogg_tell_func(void *datasource) {
	MemoryStream* stream = (MemoryStream*) datasource;
	return (long) stream->offset;
}

static int ogg_close_func(void *datasource) {
	return 0;
}

static ov_callbacks cb = { ogg_read_func, ogg_seek_func, ogg_close_func, ogg_tell_func};

/** OpenALで使用するバッファサイズを定義. */
#define AL_BUFFER_SIZE (8 * 1024)

SoundData* OggLoader::load(const char *fname) {
	AssetManager mgr = AssetManager::getInstance();
	std::vector<char> *fdata = mgr.open(fname);
	if (fdata) {
		SoundData *dat = load(fdata);
		delete fdata;
		return dat;
	}
	return NULL;
}

SoundData* OggLoader::load(std::vector<char> *data) {
	unsigned char *fdata = (unsigned char *) &(*data)[0];
	if (memcmp("RIFF", fdata, 4) == 0) {
		return loadWav(data);
	} else if (memcmp("Ogg", fdata, 3) == 0) {
		return loadOgg(data);
	} else {
		LOGE("Unsupported sound file.");
		return NULL;
	}
}

SoundData* OggLoader::loadWav(std::vector<char> *data) {
	WAVHEADER *header;
	ALenum format;
	ALsizei freq;

	// WAVのヘッダーチェック
	header = (WAVHEADER *) &(*data)[0];
	if ((memcmp("RIFF", header->riff, 4) ||
		 memcmp("WAVE", header->wave, 4) ||
		 memcmp("fmt ", header->fmt,  4) ||
		 memcmp("data", header->data, 4))) {
		LOGE("Illegal wav format.");
		return NULL;
	}

	switch (header->bitsPerSample) {
	case 8:
		format = (header->channels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
		break;
	case 16:
		format = (header->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		break;
	default:
		LOGE("Unsupported wav file - %i bitsPerSample.", header->bitsPerSample);
		return NULL;
	}
	freq = header->samplesPerSec;

	SoundData *sd = new SoundData();
	sd->data = malloc(header->dataSize);
	memcpy(sd->data, &(*data)[0], header->dataSize);
	sd->dataSize = header->dataSize;
	sd->fileType = format;
	sd->sampleRate = freq;
	return sd;
}

SoundData* OggLoader::loadOgg(std::vector<char> *data) {
	OggVorbis_File vf = {0};	//!< Oggのファイルポインタ
	MemoryStream stream = {0};	//!< 簡易メモリストリーム
	int channels;
	std::vector<char> buf;
	
	stream.offset = 0;
	stream.size = data->size();
	stream.buffer = (unsigned char *) &(*data)[0];
	
	// oggのファイルを開きます.
	if (ov_open_callbacks(&stream, &vf, NULL, 0, cb) != 0) {
		LOGE("Failed to open ogg file");
		return NULL;
	}
	
	channels = ov_info(&vf, 0)->channels;
	if (channels != 1 && channels != 2) {
		LOGE("Unsupported ogg file - %i channels. Only mono or stereo supported", channels);
		return NULL;
	}
	
	char temp[AL_BUFFER_SIZE];		//!< ストリーミングするときのバッファ
	
	// oggをデコード
	size_t decoded = 0;
	do {
		decoded = decode(&vf, temp, AL_BUFFER_SIZE);
		std::vector<char> tmp(temp, temp+decoded);
		buf.insert(buf.end(), tmp.begin(), tmp.end());
	} while (decoded != 0);
	
	
	if (buf.size() <= 0) {
		// デコードに失敗
		LOGE("No size");
		return NULL;
	}
	
	// SoundData作成
	SoundData *sd = new SoundData();
	sd->data = malloc(buf.size());
	memcpy(sd->data, &(buf)[0], buf.size());
	sd->dataSize = buf.size();
	sd->fileType = (channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
	sd->sampleRate = ov_info(&vf, -1)->rate;
	
	ov_clear(&vf);
	
	return sd;
	
}

size_t OggLoader::decode(OggVorbis_File* vf, char* buffer, int length) {
	size_t left = length;
	int bitstream = 0;
	while (left != 0) {
		long decoded = ov_read(vf, buffer, left, 0, 2, 1, &bitstream);
		if (decoded > 0) {
			buffer += decoded;
			left -= decoded;
		} else if (decoded == 0) {
			break;
		} else {
			LOGE("Ogg decoding error, will try to resume decoding");
		}
	}
	return length - left;
}
