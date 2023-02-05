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

	//��ʼ��
	initBG();
}
void mainScene::initBG() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	mainBackground = Sprite::create("../Resources/mainBackground.jpg");
	mainBackground->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));

	this->addChild(mainBackground, 0);

	//��ӿؼ�
	//�˳���Ϸ��ť
	Sprite* closeGame = Sprite::create("../Resources/closeGame.png");
	closeGame->setAnchorPoint(Point());
	closeGame->setPosition(Point(visibleSize.width-32, 0));
	this->addChild(closeGame, 2);

	/*�رհ�ť�Ĵ�������*/
	auto closeListener = EventListenerTouchOneByOne::create();
	//onTouchBegan����Ҫ�У����ұ���return true
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

	//��ʼ�ؿ���ť
	Sprite* startGame = Sprite::create("../Resources/startGame.png");
	startGame->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(startGame, 3);

	/*��ʼ��ť�Ĵ�������*/
	auto startListener = EventListenerTouchOneByOne::create();
	//onTouchBegan����Ҫ�У����ұ���return true
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

	//�֣���Ҫ����ע������¼�
	_eventDispatcher->addEventListenerWithSceneGraphPriority(closeListener, closeGame);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(startListener, startGame);
}