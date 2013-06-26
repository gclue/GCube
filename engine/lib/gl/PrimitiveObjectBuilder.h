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

#ifndef GL_POB_H
#define GL_POB_H

class Figure;

#ifdef __cplusplus
extern "C" {
#endif
    
    Figure* createTriangle(float ax, float ay, float bx, float by, float cx,
                           float cy, float z, float max, float sax, float say, float sbx,
                           float sby, float scx, float scy);
    /**
     * ポイントのFigureクラスを作成します.
     * @param x x座標
     * @param y y座標.
     * @return Figure
     */
    Figure *createPoint(float x, float y);
    Figure *createPoint2(double x, double y, double max, double width, double height, int kind);
    
    /**
     * プレートのFigureクラスを作成します.
     * @param[in] x x軸方向へのサイズ
     * @param[in] y y軸方向へのサイズ
     * @return Figure
     */
    Figure *createPlate(float x, float y);
    Figure *createPlateWithTexpos(float x, float y, float v1, float t1, float v2, float t2);
    Figure *createPlateWithRotateTexpos(float x, float y, float v1, float t1, float v2, float t2);
    
    Figure *createPlateWithParam(float sx, float sy, float ex, float ey, float z);
    Figure *createPlateWithParam2(float sx, float sy, float ex, float ey, float z, int index);
    Figure *createPlateWithParam3(float sx, float sy, float ex, float ey, float z, float max, float stx, float sty, float ttx, float tty);
    
    Figure *createTexturalPlate(float sx, float sy, float ex, float ey, float z, float max, float stx, float sty, float ttx, float tty);
    
    Figure *createLine(void);
    Figure *createLine2(float sx, float sy, float sz, float ex, float ey, float ez);
    
    
    /**
     * ボックスのFigureクラスを作成します.
     * @param[in] x x軸方向へのサイズ
     * @param[in] y y軸方向へのサイズ
     * @param[in] z z軸方向へのサイズ
     * @return Figure
     */
    Figure *createBox(float x, float y, float z);
    
    /**
     * 球のFigureクラスを作成します
     * @param[in] radius 半径
     * @param[in] slices 横に分割する数
     * @param[in] stacks 縦に分割する数
     * @return Figure
     */
    Figure *createSphere(float radius, int slices, int stacks);
    
#ifdef __cplusplus
}
#endif

#endif // end of GL_POB_H
