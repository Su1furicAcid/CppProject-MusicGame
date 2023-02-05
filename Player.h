#pragma once
#include"Entity.h"
#include<cocos2d.h>
using namespace cocos2d;

enum notesState {
	alive,dead,
};

class Player :public Entity {
public:
	CREATE_FUNC(Player);
	virtual bool init();
	void run();
	notesState noteState;
	int bornX;
};