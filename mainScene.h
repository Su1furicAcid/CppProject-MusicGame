#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class mainScene :public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(mainScene);
private:
	void initBG();
private:
	Sprite* mainBackground;
};