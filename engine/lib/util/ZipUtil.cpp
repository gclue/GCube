//
//  ZipUtil.cpp
//  GCube
//
//  Created by 小林 伸郎 on 2013/07/24.
//
//

#include "ZipUtil.h"
#include <zlib.h>
#include "Log.h"

/**
 * @define zlibで使用するバッファサイズを定義します。
 */
#define ZLIB_OUTBUF_SIZE 4096

bool ZipUtil::deflateZip(unsigned char *data, size_t len, std::string &out)
{
	char outbuf[ZLIB_OUTBUF_SIZE];
	int count;
	int flush;
	int status;
	z_stream z;
	
	// すべてのメモリ管理をライブラリに任せる
	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;
	
	// 出力ポインタ
	z.next_out = (Bytef *) outbuf;
	z.avail_out = ZLIB_OUTBUF_SIZE;
	
	// 入力ポインタ
	z.next_in = (Bytef *) data;
	z.avail_in = len;

	// zlibの初期化
	if (deflateInit2(&z, Z_DEFAULT_COMPRESSION, Z_DEFLATED, (15+16), 8, Z_DEFAULT_STRATEGY) != Z_OK) {
		return false;
	}
		
	flush = Z_FINISH;
	
	while (1) {
		status = deflate(&z, flush);
		if (status == Z_STREAM_END) {
			break; // 完了
		}
		if (status != Z_OK) {// エラー
			LOGE("error: deflate.");
			goto ERROR;
		}
		if (z.avail_out == 0) { // 出力バッファが尽きれば
			out.append(outbuf, ZLIB_OUTBUF_SIZE);
			// 出力バッファ残量を元に戻す
			z.next_out = (Bytef *) outbuf;
			z.avail_out = ZLIB_OUTBUF_SIZE;
		}
	}
	count = ZLIB_OUTBUF_SIZE - z.avail_out;
	if (count > 0) {
		out.append(outbuf, count);
	}
	
ERROR:
	return (deflateEnd(&z) == Z_OK);
}


bool ZipUtil::inflateZip(unsigned char *data, size_t len, std::string &out)
{
	char outbuf[ZLIB_OUTBUF_SIZE];
	int count;
	int status;
	z_stream z;
	
	// すべてのメモリ管理をライブラリに任せる
	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;
	
	// 出力ポインタ
	z.next_out = (Bytef *) outbuf;
	z.avail_out = ZLIB_OUTBUF_SIZE;
	
	// 入力ポインタを元に戻す
	z.next_in = (Bytef *) data;
	z.avail_in = len;
	
	if (inflateInit2(&z, 15 + 32) != Z_OK) {
		return false;
	}
	
	status = Z_OK;
	
	while (status != Z_STREAM_END) {
		status = inflate(&z, Z_NO_FLUSH);
		if (status == Z_STREAM_END) {
			break;
		}
		if (status != Z_OK) {// エラー
			LOGE("error: inflate. status=%d", status);
			goto ERROR;
		}
		if (z.avail_out == 0) { // 出力バッファが尽きれば
			out.append(outbuf, ZLIB_OUTBUF_SIZE);
			// 出力ポインタを元に戻す
			z.next_out = (Bytef *) outbuf;
			z.avail_out = ZLIB_OUTBUF_SIZE;
		}
	}
	
	// 残りを吐き出す
	count = ZLIB_OUTBUF_SIZE - z.avail_out;
	if (count > 0) {
		out.append(outbuf, count);
	}
	
ERROR:
	// 後始末
	return (inflateEnd(&z) == Z_OK);
}
