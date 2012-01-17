//
//  OggLoader.cpp
//

#include "AssetManager.h"
#include "OggLoader.h"
#include "Log.h"
#include <stdlib.h>

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
			LOGD("ERROR %ld", offset);
			return -1;
	}
	
	if (stream->offset > stream->size) {
		stream->offset = stream->size;
		LOGE("ERRORa");
		return -1;
	} else if (stream->offset < 0) {
		stream->offset = 0;
		LOGE("ERRORb");
		return -1;
	}
	return 0;
}

static long ogg_tell_func(void *datasource) {
	MemoryStream* stream = (MemoryStream*) datasource;
	return (long) stream->offset;
}

static int ogg_close_func(void *datasource) {
//	LOGE("ogg_close_func");
	return 0;
}

static ov_callbacks cb = { ogg_read_func, ogg_seek_func, ogg_close_func, ogg_tell_func};

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

