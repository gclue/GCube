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

package com.gclue.gl;

import java.util.ArrayList;

/**
 * Figureデータクラス.
 * <br><br>
 * このクラスを通じてJNI側でFigureのデータを取得します。
 * <br>
 * JNI渡すときにArrayListでは、渡せないので配列に変換しています。
 * これは、無駄があるかもしれません。
 * 
 * @author GClue, Inc.
 */
public class Figure {
	/**
	 * 頂点データ.
	 */
	private ArrayList<Float> vertices = new ArrayList<Float>();
	
	/**
	 * 法線ベクトルデータ.
	 */
	private ArrayList<Float> normals = new ArrayList<Float>();
	
	/**
	 * テクスチャデータ.
	 */
	private ArrayList<Float> textureCoords = new ArrayList<Float>();
	
	/**
	 * ライトマップデータ.
	 */
	private ArrayList<Float> lightMap = new ArrayList<Float>();
	
	/**
	 * 頂点のインデックスデータ.
	 */
	private ArrayList<Short> vertexIndexes = new ArrayList<Short>();
	
	/**
	 * 頂点データを追加します.
	 * @param tmp 追加する頂点データ
	 */
	public void addVertices(float[] tmp) {
		for (int i = 0; i < tmp.length; i++) {
			vertices.add(tmp[i]);
		}
	}
	
	/**
	 * 法線ベクトルを追加します.
	 * @param tmp 追加する法線ベクトル
	 */
	public void addNormal(float[] tmp) {
		for (int i = 0; i < tmp.length; i++) {
			normals.add(tmp[i]);
		}
	}
	
	/**
	 * テクスチャデータを追加します.
	 * @param tmp 追加するテクスチャデータ
	 */
	public void addTextureCoords(float[] tmp) {
		for (int i = 0; i < tmp.length; i++) {
			textureCoords.add(tmp[i]);
		}
	}
	
	/**
	 * ライトマップデータを追加します.
	 * @param tmp 追加するライトマップデータ
	 */
	public void addLightMap(float[] tmp) {
		for (int i = 0; i < tmp.length; i++) {
			lightMap.add(tmp[i]);
		}
	}
	
	/**
	 * 頂点のインデックスデータを追加します.
	 * @param index 頂点のインデックス
	 */
	public void addVertexIndexes(short index) {
		vertexIndexes.add(index);
	}
	
	/**
	 * 頂点データを取得します.
	 * @return 頂点データの配列
	 */
	public float[] getVertices() {
		float[] v = new float[vertices.size()];
		for (int i = 0; i < v.length; i++) {
			v[i] = vertices.get(i);
		}
		return v;
	}
	
	/**
	 * 法線ベクトルを取得します.
	 * @return 法線ベクトルの配列
	 */
	public float[] getNormal() {
		float[] v = new float[normals.size()];
		for (int i = 0; i < v.length; i++) {
			v[i] = normals.get(i);
		}
		return v;
	}

	/**
	 * テクスチャデータを取得します.
	 * @return テクスチャデータの配列
	 */
	public float[] getTextureCoords() {
		float[] v = new float[textureCoords.size()];
		for (int i = 0; i < v.length; i++) {
			v[i] = textureCoords.get(i);
		}
		return v;
	}
	
	/**
	 * 頂点のインデックスデータを取得します.
	 * @return 頂点のインデックスの配列
	 */
	public short[] getVertexIndexes() {
		short[] v = new short[vertexIndexes.size()];
		for (int i = 0; i < v.length; i++) {
			v[i] = vertexIndexes.get(i);
		}
		return v;
	}
	
	/**
	 * ライトマップのデータを取得します.
	 * @return ライトマップデータの配列
	 */
	public float[] getLightMap() {
		float[] v = new float[lightMap.size()];
		for (int i = 0; i < v.length; i++) {
			v[i] = lightMap.get(i);
		}
		return v;
	}
}
