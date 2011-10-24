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

package com.gclue.gl.ui;

import java.util.List;
import java.util.Vector;

public class GCViewGroup extends GCView {
	private List<GCView> views = new Vector<GCView>();
	
	public GCViewGroup() {
		pear = nativeCreate();
	}
	
	public void addView(GCView view) {
		nativeAddView(pear, view.pear);
		views.add(view);
		view.parent = this;
	}
	
	public void removeView(GCView view) {
		nativeRemoveView(pear, view.pear);
		views.remove(view);
	}
	
	public void removeView(int id) {
		GCView v = findViewByID(id);
		if (v != null) {
			removeView(v);
		}
	}
	
	public void removeAllView() {
	}
	
	public GCView findViewByID(int id) {
		for (int i = 0; i < views.size(); i++) {
			GCView v = views.get(i).findViewByID(id);
			if (v != null) {
				return v;
			}
		}
		return null;
	}
	
	private native int nativeCreate();
	private native void nativeAddView(int pear, int pearView);
	private native void nativeRemoveView(int pear, int pearView);
}
