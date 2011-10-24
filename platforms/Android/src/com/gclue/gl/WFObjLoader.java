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

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;

/**
 * モデルデータを読み込み、Figureクラスを作成するためのクラス.
 * @author GClue, Inc.
 */
public class WFObjLoader {
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
	 * 頂点のデータ.
	 */
	private short vertexIndex;
	
	/**
	 * 右手周りフラグ.
	 */
	private boolean rightHanded = true;
	
	/**
	 * 指定されたバッファからFigureを作成します.
	 * @param buf モデルデータ
	 * @return Figureのインスタンス
	 */
	public Figure object3D(byte[] buf) {
		return object3D(new ByteArrayInputStream(buf));
	}
	
	/**
	 * 指定されたバッファからFigureを作成します.
	 * <br><br>
	 * 読み込みに失敗した場合には、<code>null</code>を返却します。
	 * @param buf モデルデータのストリーム
	 * @return Figureのインスタンス
	 */
	public Figure object3D(InputStream in) {
		Figure figure = new Figure();
		
		// 初期化
		vertices.clear();
		normals.clear();
		textureCoords.clear();
		vertexIndex = 0;
		rightHanded = true;
		
		// データの解析
		BufferedReader reader = null;
		try {
			reader = new BufferedReader(new InputStreamReader(in), 8192);
			String line;
			while ((line = reader.readLine()) != null) {
				if (line.startsWith("f ")) {
					processFaceLine(line, figure);
				} else if (line.startsWith("v ")) {
					processVertexLine(line);
				} else if (line.startsWith("vn ")) {
					processVertexNormalLine(line);
				} else if (line.startsWith("vt")) {
					processTextureCoordLine(line);
				} else if (line.startsWith("g ")) {
				} else if (line.startsWith("usemtl ")) {
				} else if (line.startsWith("mtllib ")) {
				}
			}
		} catch (IOException e) {
			return null;
		} finally {
			try {
				reader.close();
			} catch (IOException e) {
				// do nothing.
			}
		}
		return figure;
	}
	
	/**
	 * 面情報を解析します.
	 * @param line 面情報データ
	 * @param figure データを格納するFigureクラス
	 */
	private void processFaceLine(String line, Figure figure) {
		float[] tmpV  = new float[3 * 3];
		float[] tmpVn = new float[3 * 3];
		float[] tmpVt = new float[2 * 3];
		int vsize = 3;
		int psize = 2;
		
		String[] faceStrs = line.split(" ");
		for (int i = 0; i < faceStrs.length - 1; i++) {
			String str = faceStrs[i + 1];
			String[] t = str.split("/");
			for (int j = 0; j < t.length; j++) {
				int idx = Integer.parseInt(t[j]) - 1;
				switch (j) {
				case 0:
					tmpV[i * 3 + 0] = vertices.get(idx * vsize + 0);
					tmpV[i * 3 + 1] = vertices.get(idx * vsize + 1);
					tmpV[i * 3 + 2] = vertices.get(idx * vsize + 2);
					break;
				case 1:
					if (idx < 0) {
						continue;
					}
					tmpVt[i * 2 + 0] = textureCoords.get(idx * psize + 0);
					tmpVt[i * 2 + 1] = textureCoords.get(idx * psize + 1);
					break;
				case 2:
					if (idx < 0) {
						continue;
					}
					tmpVn[i * 3 + 0] = normals.get(idx * vsize + 0);
					tmpVn[i * 3 + 1] = normals.get(idx * vsize + 1);
					tmpVn[i * 3 + 2] = normals.get(idx * vsize + 2);
					break;
				}
			}
		}
		
		// face情報を追加
		figure.addVertices(tmpV);
		figure.addNormal(tmpVn);
		figure.addTextureCoords(tmpVt);
		for (int i = 0; i < 3; i++) {
			figure.addVertexIndexes(vertexIndex);
			vertexIndex++;
		}
	}
	
	/**
	 * 頂点データを解析します.
	 * @param line 頂点データの文字列
	 */
	private void processVertexLine(String line) {
		String[] t = line.split(" ");
		float x = Float.parseFloat(t[1]);
		float y = Float.parseFloat(t[2]);
		float z = Float.parseFloat(t[3]);
		
		if (!rightHanded) {
			z = -z;
		}
		
		vertices.add(x);
		vertices.add(y);
		vertices.add(z);
	}
	
	/**
	 * 法線ベクトルのデータを解析します.
	 * @param line 法線ベクトルデータの文字列
	 */
	private void processVertexNormalLine(String line) {
		String[] t = line.split(" ");
		float x = Float.parseFloat(t[1]);
		float y = Float.parseFloat(t[2]);
		float z = Float.parseFloat(t[3]);
		
		if (!rightHanded) {
			z = -z;
		}
		
		normals.add(x);
		normals.add(y);
		normals.add(z);
	}
	
	/**
	 * テクスチャデータを解析します.
	 * @param line テクスチャデータの文字列
	 */
	private void processTextureCoordLine(String line) {
		String[] t = line.split(" ");
		float x = Float.parseFloat(t[1]);
		float y = Float.parseFloat(t[2]);
		
		textureCoords.add(x);
		textureCoords.add(y);
	}
}
