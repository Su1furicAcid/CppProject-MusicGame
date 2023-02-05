#ifndef _initScene_H_
#define _initScene_H_
#include "cocos2d.h"
using namespace cocos2d;
class Player;
class initScene : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(initScene);
private:
	void initBG();
private:
	Sprite* initBackground;
	Player* player;
};
#endif
