#pragma once
#include"cocos2d.h"
#include"Controller.h"
using namespace cocos2d;

class SimpleMoveController :public Controllers {
public:
	CREATE_FUNC(SimpleMoveController);
	virtual bool init();
	virtual void update(float dt);
	void setiSpeed(int iSpeed);
private:
	int m_iSpeed;
};