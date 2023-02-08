#pragma once
#pragma once
#include"cocos2d.h"
#include"cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

using namespace cocos2d;
class GameQuit : public Layer {
public:
	virtual bool init();
	CREATE_FUNC(GameQuit);
	static Scene* scene(RenderTexture* sqr);
	void menuContinueCallback(Object* pSender);
	void quitGame(Object* pSender);
};
