#include "initScene.h"
#include "mainScene.h"
using namespace cocos2d;
Scene* initScene::createScene() {
	auto scene = Scene::create();
	auto layer = initScene::create();
	scene->addChild(layer);
	return scene;
}
bool initScene::init() {
	if (!Layer::init())return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	/*游戏标题图片*/
	Sprite* titleSprite = Sprite::create("../Resources/PROMISE.png");
	titleSprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(titleSprite, 2);

	/*初始化背景图片*/
	initBG();

	/*屏幕触摸事件监听*/
	auto listener = EventListenerTouchOneByOne::create();
	//onTouchBegan必须要有，而且必须return true
	listener->onTouchBegan = [](Touch* touch, Event* event) {    
		Point pos = touch->getLocation();
		Point pos1 = touch->getLocation();        /* 获取单击坐标，基于3D */
		Point pos2 = touch->getLocationInView();   /* 获取单击坐标，基于2D */
		Point pos3 = Director::getInstance()->convertToGL(pos2);   /* 获取单击坐标，基于Cocos2d-x */
		log("HelloWorldScene onTouchBegan! pos1 x=%f, y=%f", pos1.x, pos1.y);
		log("HelloWorldScene onTouchBegan! pos2 x=%f, y=%f", pos2.x, pos2.y);
		log("HelloWorldScene onTouchBegan! pos3 x=%f, y=%f", pos3.x, pos3.y);
		return true;
	};
	listener->onTouchMoved = [](Touch* touch, Event* event) {
		log("HelloWorldScene onTouchMoved");
	};
	listener->onTouchEnded = [=](Touch* touch, Event* event) {
		log("HelloWorldScene onTouchEnded");
		auto scene = mainScene::createScene();
		TransitionFade* trans = TransitionFade::create(1,scene);
		Director::getInstance()->replaceScene(trans);
	};

	//乐，不要忘了注册监听事件
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, initScene::initBackground);

}

void initScene::initBG() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	initBackground = Sprite::create("../Resources/initBackground.jpg");
	initBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(initBackground, 0);
}