package com.gclue.util;

public final class StringUtils {
	private StringUtils() {
	}
	
	/**
	 * "./"や"../"などを調整してURLを修正する. 
	 * <br>
	 * @param url リソースがあるURL
	 * @return String 絶対パスにして取得する
	 */
	public static String alterURL(String url) {
		int idx;
		String str1;
		String str3;
		
		while ((idx = url.indexOf("/./")) != -1) {
			str1 = url.substring(0, idx);
			str3 = url.substring(idx + 2);
			url = str1 + str3;
		}
		
		while ((idx = url.indexOf("/../")) != -1) {
			str1 = url.substring(0, idx);
			str3 = url.substring(idx + 3);
			int i;
			if ((i = str1.lastIndexOf('/')) != -1) {
				str1 = str1.substring(0, i);
			} else {
				// TODO: ディレクトリ指定が誤っていたときの処理
				// throw(new Exception("Util#alterURL: ../の前にディレクトリが見つかりません"));
				return url;
			}
			url = str1 + str3;
		}
		return url;
	}

	/**
	 * 特殊文字を置換しするメソッド.
	 * 
	 * <p>
	 * URLに含まれる<%USER%>, <%VER%>等を処理する。
	 * </p>
	 * 
	 * @param url 変換する文字列
	 * 
	 * @return 変換された文字列
	 */
	public static String changeURL(String url) {
		return alterURL(url);
	}
}
