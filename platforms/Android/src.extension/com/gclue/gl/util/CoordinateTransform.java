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

package com.gclue.gl.util;

public final class CoordinateTransform {
	
	private static float baseWidth = 480.0f;
	private static float baseHeight = 800.0f;
	
	private static float halfBaseWidth = baseWidth / 2.0f;
	private static float halfBaseHeight = baseHeight / 2.0f;
	
	private static int width = 480;
	private static int height = 800;
	private static float aspect = width / (float) height;
	
	private CoordinateTransform() {
	}
	
	public static void setDisplaySize(int width, int height) {
		CoordinateTransform.width = width;
		CoordinateTransform.height = height;
		CoordinateTransform.aspect = width / (float) height;
	}
	
	public static Pointf toGLPoint(int x, int y) {
		float xx = x * baseWidth / width;
		float yy = y * baseHeight / height;
		Pointf p = new Pointf();
		p.x = (xx / halfBaseWidth) - 1.0f;
		p.y = (((-yy) / halfBaseHeight) + 1.0f) / aspect;
		return p;
	}
	
	public static Point toJavaPoint(float x, float y) {
		float xx = (x + 1) * halfBaseWidth;
		float yy = (-y + 1) * baseHeight;
		int px = (int) (xx * width / baseWidth);
		int py = (int) (yy * height / baseHeight);
		Point p = new Point();
		p.x = px;
		p.y = py;
		return p;
	}
}
