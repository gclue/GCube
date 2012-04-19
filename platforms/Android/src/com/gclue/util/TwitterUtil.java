package com.gclue.util;

import twitter4j.Twitter;
import twitter4j.TwitterException;
import twitter4j.TwitterFactory;
import twitter4j.auth.AccessToken;
import twitter4j.auth.OAuthAuthorization;
import twitter4j.auth.RequestToken;
import twitter4j.conf.Configuration;
import twitter4j.conf.ConfigurationBuilder;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.TranslateAnimation;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

/**
 * ツイッター連携をするUtilクラス.
 * @author haga
 *
 */
public class TwitterUtil {

	/**
	 * ログタグ.
	 */
	private static final String TAG = "com.gclue.util.TwitterUtil";
	
	/**
	 * SharedPreferencesのDefault値.
	 */
	private static final String NO_DATA = "NO_PREFERENCES_DATA";
	
	/**
	 * OAuthコンシューマーキー.
	 */
	private static final String CONSUMER_KEY = "REPLACE_ME"; //Twitterサイトから取得したcomsuer key.
	
	/**
	 * OAuthコンシューマーシークレット.
	 */
	private static final String CONSUMER_SECRET = "REPLACE_ME"; // Twitterサイトから取得したconsumer_secret
	
	/**
	 * コールバックURL.
	 */
	private static final String CALLBACK_URL = "REPLACE_ME"; // TwitterサイトからコールバックされるURL
	
	/**
	 * SharedPreferencesにセーブするための名前.
	 */
	private static final String PREFERENCE_NAME = "GCubeTwitterOAuthPreference";
	
	
	public static final int TWITTER_RESPONSE_POST_SUCCESS = 2;
	public static final int TWITTER_RESPONSE_POST_FAILED = 3;
	public static final int TWITTER_RESPONSE_AUTH_SUCCESS = 0;
	public static final int TWITTER_RESPONSE_AUTH_FAILED = 1;
	

	/**
	 * コンテキスト.
	 */
	private Context context;
	
	/**
	 * ルートレイアウト.
	 */
	private FrameLayout root;
	
	/**
	 * OAuthリクエストトークン.
	 */
	private RequestToken requestToken;

	/**
	 * イベントを受け取るインターフェースクラス.
	 * @author haga
	 *
	 */
	public static interface OnTwitterResponseListener {
		/**
		 * ツイッターイベントのレスポンスを受け取ります.
		 * @param type レスポンスタイプ.
		 */
		void onTwitterResponse(int type);
	}

	

	
	/**
	 * ツイッターインスタンス.
	 */
	private Twitter twitter;
	
	/**
	 * OAuth認証インスタンス.
	 */
	private OAuthAuthorization mOauth;
	
	/**
	 * WebViewとツールバーを表示するレイアウト.
	 */
	private LinearLayout base;

	/**
	 * トークン.
	 */
	private String oauthToken = "";
	
	/**
	 * ベリファイアー.
	 */
	private String oauthVerifier = "";

	/**
	 * コンストラクタ.
	 * @param context コンテキスト.
	 * @param root メインビューのルート.
	 */
	public TwitterUtil(Context context, FrameLayout root) {
		this.context = context;
		this.root = root;
	}

	
	
	
	/**
	 * ツイッターのインスタンスを作成して取得します.
	 * @param oauthToken OAuthトークン.
	 * @param oauthTokenSecret OAuthトークンシークレット.
	 * @return インスタンス.
	 */
	private Twitter getTwitterInstance(String oauthToken,
			String oauthTokenSecret) {
		// ツイートするためのTwitterインスタンスを作ります。
		// ConsumerKey, Secret, OAuthToken,
		// TokenSecretの４つをセットしcofigurationBuilderをビルド
		ConfigurationBuilder conf = new ConfigurationBuilder();
		conf.setOAuthConsumerKey(CONSUMER_KEY);
		conf.setOAuthConsumerSecret(CONSUMER_SECRET);
		conf.setOAuthAccessToken(oauthToken);
		conf.setOAuthAccessTokenSecret(oauthTokenSecret);
		return new TwitterFactory(conf.build()).getInstance();
	}
	
	/**
	 * ツイートを行います.
	 * @param text ツイートする文.
	 * @param listener ツイート結果を受け取るリスナー.
	 */
	public void tweet(final String text, final OnTwitterResponseListener listener) {
		
		//認証済みであればそのままツイート.
		if (isAuthorized()) {
			try {
				//ツイート
				//認証済みかを確認するところでインスタンスは生成されます.
				twitter.updateStatus(text);
				//ツイート正常完了のレスポンスを返して終了
				listener.onTwitterResponse(TWITTER_RESPONSE_POST_SUCCESS);
			} catch (TwitterException e) {
				//ツイート以上終了のレスポンスを返す.
				listener.onTwitterResponse(TWITTER_RESPONSE_POST_FAILED);
				e.printStackTrace();
			}
		} else {
			//認証済みでなければ、認証を行う。
			authenticate(new OnTwitterResponseListener() {

				@Override
				public void onTwitterResponse(int type) {
					//この認証が済んだら、もう一度ツイートを実行する.
					if(type == TWITTER_RESPONSE_AUTH_SUCCESS) {
						tweet(text, listener);
					}else {
						//認証が失敗したらレスポンスを返して終了.
						listener.onTwitterResponse(TWITTER_RESPONSE_POST_FAILED);
					}
				}
				
			});
		}
	}

	/**
	 * 認証が済んでいるかを確認します.
	 * <br>
	 * 認証が済んでいる場合はtwitterのインスタンスを作成し,trueを返します.
	 * @return 認証が済んでいるか.
	 */
	private boolean isAuthorized() {
		//SharedPreferencesから既に登録されて保存されたトークンを取得します.
		SharedPreferences pref = context.getSharedPreferences(PREFERENCE_NAME,
				0);
		String oauthToken = pref.getString("oauth_token", NO_DATA);
		String oauthTokenSecret = pref.getString("oauth_token_secret", NO_DATA);

		// Preferenceに何もなかったら認証をしていないです.
		if (oauthToken.equals(NO_DATA) || oauthTokenSecret.equals(NO_DATA)) {
			return false;
		} else {
			//認証が済んでいたらtwitterのインスタンスを作成します.
			twitter = getTwitterInstance(oauthToken, oauthTokenSecret);
			return true;
		}
	}

	/**
	 * 認証処理を行います.
	 * <br>
	 * 認証結果は引数のリスナーが受け取ります.
	 * @param listener 認証結果を受け取るリスナー.
	 */
	public void authenticate(OnTwitterResponseListener listener) {

		//認証が済んでいたらレスポンスを返します.
		if (isAuthorized()) {
			listener.onTwitterResponse(TWITTER_RESPONSE_AUTH_SUCCESS);
			return;
		}

		//コンシューマキー, シークレットを元にOAuth認証インスタンスを生成します.
		ConfigurationBuilder cbuilder = new ConfigurationBuilder();
		cbuilder.setOAuthConsumerKey(CONSUMER_KEY);
		cbuilder.setOAuthConsumerSecret(CONSUMER_SECRET);
		Configuration conf = cbuilder.build();
		mOauth = new OAuthAuthorization(conf);
		mOauth.setOAuthAccessToken(null); // これをやらないと下記getOAuthRequestToken()で例外が発生する
		String authUrl = null;

		try {
			//インスタンスからリクエストトークン、OAtuh認証URLを取得します.
			requestToken = mOauth.getOAuthRequestToken(CALLBACK_URL);
			authUrl = requestToken.getAuthorizationURL();
		} catch (Exception e) {
			Log.e(TAG, e.toString());
			//エラーを返して終了.
			listener.onTwitterResponse(TWITTER_RESPONSE_AUTH_FAILED);
			return;
		}

		if (authUrl != null) {
			//URLがあれば、WebViewを表示します.
			openWebView(authUrl, listener);
		}else {
			//URLが存在しなかったのでエラーを返して終了
			listener.onTwitterResponse(TWITTER_RESPONSE_AUTH_FAILED);
			return;
		}

	}

	/**
	 * 認証用のWebViewを開きます.
	 * @param authUrl OAuth認証URL.
	 * @param listener 認証レスポンス用のリスナー.
	 */
	private void openWebView(final String authUrl, final OnTwitterResponseListener listener) {

		root.post(new Runnable() {

			@Override
			public void run() {
				
				base = new LinearLayout(context);
				base.setOrientation(LinearLayout.VERTICAL);
				
				LinearLayout toolbar = new LinearLayout(context);
				toolbar.setBackgroundColor(Color.WHITE);
				Button btBack = new Button(context);
				btBack.setText("Cancel");
				btBack.setVisibility(View.VISIBLE);
				btBack.setOnClickListener(new OnClickListener() {

					@Override
					public void onClick(View arg0) {
						//キャンセルボタン時の処理.
						closeViews(1);
					}

				});

				toolbar.addView(btBack);

				WebView webView = new WebView(context);
				webView.setWebViewClient(new TwitterWebViewClient(listener));

				//WebView90%:ToolBar10%のビューを作成します.
				base.addView(webView, new LayoutParams((int) root.getWidth(),
						(int) (root.getHeight() * 0.9)));
				base.addView(toolbar, new LayoutParams((int) root.getWidth(),
						(int) (root.getHeight() * 0.1)));

				root.addView(base);

				//下から上に競り上がるアニメーション.0.4s
				TranslateAnimation anim = new TranslateAnimation(0, 0, root
						.getHeight(), 0);
				anim.setDuration(400);
				base.startAnimation(anim);

				webView.loadUrl(authUrl);
			}

		});

	}
	
	/**
	 * ビューを閉じます.
	 * <br>
	 * type = 0で認証成功時のビュークローズ.<br>
	 * type = 1でボタンで閉じたときのビュークローズイベント.
	 * @param type タイプ.
	 */
	public void closeViews(int type) {
		closeViews(type, null);
	}
	
	/**
	 * ビューを閉じます.
	 * type = 0で認証成功時のビュークローズ.<br>
	 * type = 1でボタンで閉じたときのビュークローズイベント.
	 * @param type タイプ.
	 * @param listener 認証成功時のレスポンスリスナー.
	 */
	public void closeViews(int type, OnTwitterResponseListener listener) {
		
		//閉じるアニメーション.
		//上から下に0.4sでスライド.
		TranslateAnimation anim = new TranslateAnimation(0, 0, 0,
				root.getHeight());
		anim.setDuration(400);
		anim.setAnimationListener(new AnimationListener() {

			@Override
			public void onAnimationEnd(Animation arg0) {
				//アニメーションが終了したらビューの削除.
				root.removeView(base);
			}

			@Override
			public void onAnimationRepeat(Animation arg0) {
			}

			@Override
			public void onAnimationStart(Animation arg0) {
			}

		});
		base.startAnimation(anim);

		
		//認証成功時の処理.
		if (type == 0) {
			AccessToken accessToken;
			try {
				if(twitter == null) {
					twitter = getTwitterInstance(null, null);
				}
				//アクセストークンの取得.
				accessToken = twitter.getOAuthAccessToken(requestToken,
						oauthVerifier);
				
				// 連携状態とトークンをSharedPreferencesに登録します.
				SharedPreferences pref = context.getSharedPreferences(
						PREFERENCE_NAME, 0);
				SharedPreferences.Editor editor = pref.edit();
				editor.putString("oauth_token", accessToken.getToken());
				editor.putString("oauth_token_secret",
						accessToken.getTokenSecret());
				editor.putString("raw_token", oauthToken);
				editor.putString("raw_secret", oauthVerifier);
				editor.putString("status", "available");
				editor.commit();
				
				if(listener != null) {
					//認証成功のレスポンスを返します.
					listener.onTwitterResponse(TWITTER_RESPONSE_AUTH_SUCCESS);
				}
			} catch (TwitterException e) {
				if(listener != null) {
					//認証失敗のレスポンスを返します.
					listener.onTwitterResponse(TWITTER_RESPONSE_AUTH_FAILED);
				}
				e.printStackTrace();
			}
		}

	}

	/**
	 * Twitter認証を行うWebViewClientクラス.
	 * @author haga
	 *
	 */
	private class TwitterWebViewClient extends WebViewClient {

		/**
		 * レスポンスを送信するリスナー.
		 */
		private OnTwitterResponseListener listener;
		
		/**
		 * コンストラクタ.
		 * @param listener レスポンスリスナー.
		 */
		public TwitterWebViewClient(OnTwitterResponseListener listener) {
			super();
			this.listener = listener;
		}

		@Override
		public boolean shouldOverrideUrlLoading(WebView view, String url) {
			
			// 認証が完了したら、指定したCallbackURLにリダイレクトされる。
			// このとき、URLの頭がCallbackURLであれば、認証成功したはずなので、その処理を以下に書く
			if (url != null && url.startsWith(CALLBACK_URL)) {

				// リダイレクトされたURLはトークンなどの情報を持っているため、情報をURLから取得
				String[] urlParameters = url.split("\\?")[1].split("&");
				oauthToken = "";
				oauthVerifier = "";

				// oauth_tokenをURLから切り出す。
				if (urlParameters[0].startsWith("oauth_token")) {
					oauthToken = urlParameters[0].split("=")[1];
				} else if (urlParameters[1].startsWith("oauth_token")) {
					oauthToken = urlParameters[1].split("=")[1];
				}

				// oauth_verifierをURLから切り出す。
				if (urlParameters[0].startsWith("oauth_verifier")) {
					oauthVerifier = urlParameters[0].split("=")[1];
				} else if (urlParameters[1].startsWith("oauth_verifier")) {
					oauthVerifier = urlParameters[1].split("=")[1];
				}

				//ビューを認証完了として閉じる.
				closeViews(0, listener);
			}
			return super.shouldOverrideUrlLoading(view, url);
		}

	}

}
