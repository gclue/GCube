//
//  ZipUtil.h
//  GCube
//
//  Created by 小林 伸郎 on 2013/07/24.
//
//

#ifndef __GCube__ZipUtil__
#define __GCube__ZipUtil__

#include <string>
#include <vector>

/**
 * Zipを処理するためのクラス.
 */
class ZipUtil {
private:
	ZipUtil() {}
public:
	/**
	 * 指定されたバイナリをBase64の文字列にエンコードします.
	 * @param[in] src エンコードするバイナリデータ
	 * @param[out] dst エンコードされた文字列
	 * @return エンコードに成功した場合はtrue, それ以外はfalse
	 */
	static bool inflateZip(unsigned char *data, size_t len, std::string &out);
	
	/**
	 * 指定された文字列をBase64でデコードします.
	 * @param[in] src デコードする文字列
	 * @param[out] dst デコードされたバイナリデータ
	 * @return デコードに成功した場合はtrue, それ以外はfalse
	 */
	static bool deflateZip(unsigned char *data, size_t len, std::string &out);
};



#endif /* defined(__GCube__ZipUtil__) */
