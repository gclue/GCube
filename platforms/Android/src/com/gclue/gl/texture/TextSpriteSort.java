/*
 * The MIT License (MIT)
 * Copyright (c) 2011 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package com.gclue.gl.texture;

import java.util.List;

import android.graphics.Rect;

public class TextSpriteSort implements ISpriteSortable {
	private List<Sprite> list;
	
	@Override
	public boolean sort(List<Sprite> list, Rect rect) {
		this.list = list;
		testPlacement(rect.left, rect.top, rect.right, rect.bottom);
		return false;
	}
	
	public void testPlacement(int x, int y, int width, int height) {
		Sprite sprite = getLargestWidth(width, height);
		if (sprite == null) {
			return;
		}
		sprite.setPosition(x, y);
		sprite.setDecision(true);
		
		int w = width - sprite.getWidth();
		while (w > 0) {
			Sprite s = getLargestWidth(w, height);
			if (s == null) {
				break;
			}
			
			int xx = x + width - w;
			int yy = y;
			
			s.setPosition(xx, yy);
			s.setDecision(true);
			
			w -= s.getWidth();
		}
		
		y += sprite.getHeight();
		height -= sprite.getHeight();
		
		testPlacement(x, y, width, height);
	}
	
	public Sprite getLargestWidth(int w, int h) {
		Sprite s = null;
		for (int i = 0; i < list.size(); i++) {
			Sprite sprite = list.get(i);
			if (s == null || s.getWidth() < sprite.getWidth()) {
				if (!sprite.isDecision() && sprite.getWidth() < w && sprite.getHeight() < h) {
					s = sprite;
				}
			}
		}
		return s;
	}
}
