#pragma once
#include"cocos2d.h"
#include"Controller.h"
using namespace cocos2d;
class Entity :public Node ,public ControllerListener{
public:
	void bindSprite(Sprite* sprite);
	void setController(Controllers* controller);
	virtual void setTagPosition(int x, int y);
	virtual Point getTagPosition();
protected:
	Sprite* m_sprite;
	Controllers* m_controller;
};