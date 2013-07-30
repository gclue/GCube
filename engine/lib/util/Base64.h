//
//  Base64.h
//  GCube
//
//  Created by 小林 伸郎 on 2013/07/24.
//
//

#ifndef __GCube__Base64__
#define __GCube__Base64__

#include <string>
#include <vector>

/**
 * Base64を処理するためのクラス.
 */
class Base64 {
private:
	Base64() {}
public:
	/**
	 * 指定されたバイナリをBase64の文字列にエンコードします.
	 * @param[in] src エンコードするバイナリデータ
	 * @param[out] dst エンコードされた文字列
	 * @return エンコードに成功した場合はtrue, それ以外はfalse
	 */
	static bool encode(const std::vector<unsigned char>& src, std::string& dst);
	
	/**
	 * 指定された文字列をBase64でデコードします.
	 * @param[in] src デコードする文字列
	 * @param[out] dst デコードされたバイナリデータ
	 * @return デコードに成功した場合はtrue, それ以外はfalse
	 */
	static bool decode(const std::string& src, std::vector<unsigned char>& dst);
};

#endif /* defined(__GCube__Base64__) */
