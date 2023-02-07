#pragma once
#include"cocos2d.h"
using namespace cocos2d;
class GamePause : public Layer {
public:
	virtual bool init();
	CREATE_FUNC(GamePause);
	static Scene* scene(RenderTexture* sqr);
	void menuContinueCallback(Object* pSender);
};