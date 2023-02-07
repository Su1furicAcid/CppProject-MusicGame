#pragma once
#include"cocos2d.h"
#include"cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

using namespace cocos2d;
class GamePause : public Layer {
public:
	virtual bool init();
	CREATE_FUNC(GamePause);
	static Scene* scene(RenderTexture* sqr);
	void menuContinueCallback(Object* pSender);
	static float volumeFloat;
	ControlSlider* volumeController;
	void volumeChanged(Object* pSender, Control::EventType controlEvent);
	static float getVolumeFloat();
};
