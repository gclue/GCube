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

package com.gclue.gl.scene;

import com.gclue.gl.ui.GCView;

public class GCLayer2D extends GCLayer {
	protected GCView root;
	protected GCView dialog;
	
	public GCLayer2D() {
		pear = nativeCreate();
	}

	public void setContentView(GCView view) {
		this.root = view;
	}
	
	public GCView getContentView() {
		return root;
	}
	
	public GCView findViewByID(int id) {
		if (root == null) {
			return null;
		}
		return root.findViewByID(id);
	}
	
	public void openDialog(GCView view) {
		if (view == null) {
			return;
		}
		nativeOpenDialog(pear, view.getPear());
		this.dialog = view;
	}
	
	public void closeDialog() {
		nativeCloseDialog(pear);
		this.dialog = null;
	}
	
	private native int nativeCreate();
	private native void nativeSetContentView(int pear, int pearView);
	private native void nativeOpenDialog(int pear, int pearView);
	private native void nativeCloseDialog(int pear);
}
