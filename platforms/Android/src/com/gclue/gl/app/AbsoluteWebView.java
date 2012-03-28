package com.gclue.gl.app;

import android.content.Context;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class AbsoluteWebView extends ViewGroup {

	WebView webView;
	int posx;
	int posy;
	
	int sizex;
	int sizey;
	
	public AbsoluteWebView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		
		webView = new WebView(context);
		webView.getSettings().setJavaScriptEnabled(true);
		WebViewClient wc = new WebViewClient();
		webView.setWebViewClient(wc);
		
		this.addView(webView);
		
	}

	
	public void setWebViewPosition(int x, int y) {
		posx = x;
		posy = y;
	}
	
	public void setWebViewSize(int x, int y) {
		sizex = x;
		sizey = y;
	}
	public void loadUrl(String url) {
		webView.loadUrl(url);
	}
	
	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		// TODO Auto-generated method stub
		
		webView.layout(posx - sizex/2, posy - sizey/2, posx + sizex/2, posy + sizey/2);
		
	}

	
}
