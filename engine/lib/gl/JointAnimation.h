//
//  JointAnimation.h
//  GCube
//
//  Created by 小林 伸郎 on 2013/01/08.
//
//

#ifndef __GCube__JointAnimation__
#define __GCube__JointAnimation__

#include "glcommon.h"
#include <vector>
#include <string>


class JointKeyFrame {
public:
	std::string sid;				//!< キーフレームが対応するJointのSID
	std::vector<float> times;		//!< キーフレームの時間
	std::vector<Matrix3D*> matrixs;	//!< マトリックス
	
	JointKeyFrame();
	~JointKeyFrame();

	/**
	 * キーフレームの時間を追加します.
	 */
	void addTime(const float *v, int len);
	
	/**
	 * キーフレームに対応するマトリックスを追加します.
	 */
	void addMatrix(const float *v, int len);
	
	/**
	 * 指定された時間に合うマトリクスを取得します.
	 * 時間がアニメーションの時間を超えていた場合には、最後のマトリクスを返します.
	 * @param[in] time 時間
	 * @return マトリクス
	 */
	Matrix3D* getMatrix(float time);
	
	/**
	 * アニメーションの時間を取得します.
	 */
	float getAnimationTime();
};

class JointAnimation {
public:
	std::vector<JointKeyFrame*> animations;	//!< キーフレーム一覧
	
	float animationTime;		//!< アニメーション時間
	bool repeat;				//!< アニメーションの繰り返し再生フラグ
	
	JointAnimation();
	~JointAnimation();
	
	/**
	 * アニメーションしている時間を取得します.
	 * @return アニメーション時間
	 */
	float getAnimationTime();
	
	/**
	 * アニメーションのトータル時間を取得します.
	 * @return トータル時間
	 */
	float getTotalAnimationTime();
};

#endif /* defined(__GCube__JointAnimation__) */
