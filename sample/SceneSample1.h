//
//  SceneSample1.h
//  GCube
//
//  Created by  on 12/04/20.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef GCube_SceneSample1_h
#define GCube_SceneSample1_h

#include "Scene.h"
#include "Button.h"

class SceneSample1 : public Scene , public OnButtonClickListener {
private:
	typedef Scene super;
    
    
public:
	/**
	 * コンストラクタ.
	 * @param[in] controller コントローラー
	 */
	SceneSample1(ApplicationController *controller);
    
	/**
	 * デストラクタ.
	 */
	virtual ~SceneSample1();
    
	//////////////////////////////////////////////////////////
	// IScene の実装
	//////////////////////////////////////////////////////////
	
	/**
	 * タッチイベント.
	 * @param[in] event タッチイベント
	 */
	virtual bool onTouchEvent(TouchEvent &event);
    
	//////////////////////////////////////////////////////////
	// OnButtonClickListener の実装
	//////////////////////////////////////////////////////////

    virtual void onButtonClick(Button *btn);
    virtual void onButtonTouchStart(Button *btn);
};



#endif
