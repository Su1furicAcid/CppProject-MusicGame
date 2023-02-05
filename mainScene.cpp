#include"mainScene.h"
#include"gameScene.h"
#include"cocos2d.h"
using namespace cocos2d;
Scene* mainScene::createScene() {
	auto scene = Scene::create();
	auto layer = mainScene::create();
	scene->addChild(layer);
	return scene;
}
bool mainScene::init() {
	if (!Layer::init())return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//初始化
	initBG();
}
void mainScene::initBG() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	mainBackground = Sprite::create("../Resources/mainBackground.jpg");
	mainBackground->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));

	this->addChild(mainBackground, 0);

	//添加控件
	//退出游戏按钮
	Sprite* closeGame = Sprite::create("../Resources/closeGame.png");
	closeGame->setAnchorPoint(Point());
	closeGame->setPosition(Point(visibleSize.width-32, 0));
	this->addChild(closeGame, 2);

	/*关闭按钮的触摸监听*/
	auto closeListener = EventListenerTouchOneByOne::create();
	//onTouchBegan必须要有，而且必须return true
	closeListener->onTouchBegan = [](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point pos = Director::getInstance()->convertToGL(touch->getLocationInView());
		if (target->getBoundingBox().containsPoint(pos)) {
			target->setOpacity(100);
		}
		log("Starthere");
		return true;
	};
	closeListener->onTouchMoved = [](Touch* touch, Event* event) {
		log("Movinghere");
	};
	closeListener->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		target->setOpacity(255);
		Point pos = Director::getInstance()->convertToGL(touch->getLocationInView());
		if (target->getBoundingBox().containsPoint(pos)) {
			Director::getInstance()->end();
		}
		log("StopHere");
	};

	//开始关卡按钮
	Sprite* startGame = Sprite::create("../Resources/startGame.png");
	startGame->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(startGame, 3);

	/*开始按钮的触摸监听*/
	auto startListener = EventListenerTouchOneByOne::create();
	//onTouchBegan必须要有，而且必须return true
	startListener->onTouchBegan = [](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point pos = Director::getInstance()->convertToGL(touch->getLocationInView());
		if (target->getBoundingBox().containsPoint(pos)) {
			target->setOpacity(100);
		}
		log("Starthere");
		return true;
	};
	startListener->onTouchMoved = [](Touch* touch, Event* event) {
		log("Movinghere");
	};
	startListener->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		target->setOpacity(255);
		Point pos = Director::getInstance()->convertToGL(touch->getLocationInView());
		if (target->getBoundingBox().containsPoint(pos)) {
			auto scene = gameScene::createScene();
			TransitionFade* trans = TransitionFade::create(1,scene);
			Director::getInstance()->replaceScene(trans);
		}
		log("StopHere");
	};

	//乐，不要忘了注册监听事件
	_eventDispatcher->addEventListenerWithSceneGraphPriority(closeListener, closeGame);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(startListener, startGame);
}