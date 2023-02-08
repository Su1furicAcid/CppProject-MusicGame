#include "cocos2d.h"
#include "gameScene.h"
#include "CallbackTimeCounter.h"
#include "Player.h"
#include "Entity.h"
#include "SimpleMoveController.h"
#include <vector>
#include "PhysicsRippleSprite.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "AudioEngine.h"
#include "GamePause.h"
#include "GameQuit.h"
using namespace cocos2d;
using namespace experimental;
Scene* gameScene::createScene() {
	auto scene = Scene::create();
	auto layer = gameScene::create();
	scene->addChild(layer);
	return scene;
}
bool gameScene::init() {
	if (!Layer::init())return false;
	initBG();
	timeCounter();
	createUI();
	return true;
}
void gameScene::initBG() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	gameBackground = PhysicsRippleSprite::create("../Resources/gameBackground.jpg");
	gameBackground->setPosition(Point(0, 0));
	this->addChild(gameBackground);
}

void gameScene::noteBorn() {
	Sprite* note = Sprite::create("../Resources/note.png");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Player* mNote = Player::create();
	mNote->bindSprite(note);
	int randomX = 1+rand() % 390;
	mNote->setPosition(Point(randomX, visibleSize.height));
	mNote->noteState = alive;
	mNote->bornX = randomX;
	this->addChild(mNote);
	//log("%d %d", randomX, mNote->bornX);
	SimpleMoveController* simpleMoveControl1 = SimpleMoveController::create();
	simpleMoveControl1->setiSpeed(-5);
	this->addChild(simpleMoveControl1);
	mNote->setController(simpleMoveControl1);
	Notes.pushBack(mNote);
	MoveControllers.pushBack(simpleMoveControl1);
}

void gameScene::loadData() {

}

void gameScene::timeCounter() {
	/* 在3.0之前的版本使用CCArray 之后的版本更推荐使用Vector
	Array* timePointArr = Array::create();
	CC_SAFE_RETAIN(timePointArr);*/
	std::string str = FileUtils::getInstance()->getStringFromFile("TimeData.json");
	rapidjson::Document timeData;
	timeData.Parse<0>(str.c_str());
	if (timeData.HasParseError()) {
		return;
	}
	for (int i = 0; i < timeData.Size(); i++) {
		timePointArray.push_back(timeData[i]["value"].GetFloat());
	}
	CallbackTimeCounter* cbTimeCounter = CallbackTimeCounter::create();
	this->addChild(cbTimeCounter);
	cbTimeCounter->start(timePointArray, [&]() {
		noteBorn();
	});
	this->schedule(schedule_selector(gameScene::noteLifeListener), 0.15f);
}

void gameScene::noteLifeListener(float dt) {
	//log("ReachHere");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	for (auto& i : Notes) {
		if (i->getTagPosition().y <= visibleSize.height/2-350 && i->noteState==alive) {
			//log("CatchIt!");
			calCombo(i->getTagPosition(), 1);
			noteRemove(i,2);
			missNum++;
		}
	}
}

void gameScene::noteRemove(Player* pnote,int Type) {
	Player* deadNote = pnote;
	if (deadNote == NULL)return;
	deadNote->noteState = dead;
	Sprite* sprite;
	if (Type == 1) {
		sprite = Sprite::create("../Resources/true.png");
		successEffectID = AudioEngine::play2d("success_sound.mp3");
		AudioEngine::setVolume(successEffectID, volume);
	}
	if (Type == 2) {
		sprite = Sprite::create("../Resources/false.png");
		failEffectID = AudioEngine::play2d("fail_sound.mp3");
		AudioEngine::setVolume(failEffectID, volume);
	}
	Player* afterNote = Player::create();
	afterNote->bindSprite(sprite);
	afterNote->setPosition(deadNote->getTagPosition());
	this->addChild(afterNote);
	afterNote->setCascadeOpacityEnabled(true);
	ActionInterval* forwardOut = FadeOut::create(1.0f);
	Action* actionOut = Sequence::create(forwardOut, NULL);
	afterNote->runAction(actionOut);
	deadNote->removeFromParent();
	//log("removeSuccess");
}

void gameScene::createUI() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* checkLine = Sprite::create("../Resources/line.png");
	checkLine->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 250));
	this->addChild(checkLine, 10);
	greatRect = CCRectMake(0, visibleSize.height / 2 - 300, visibleSize.width, 100);
	perfectRect= CCRectMake(0, visibleSize.height / 2 - 275, visibleSize.width, 50);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		gameScene::TouchesBegan(touch,event);
		return true;
	};
	listener->onTouchMoved = [](Touch* touch, Event* event) {

	};
	listener->onTouchEnded = [=](Touch* touch, Event* event) {
		gameScene::TouchesEnded(touch, event);
		//log("successTouch!");
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, gameBackground);
	listener->setSwallowTouches(true);
	gameScore = Label::createWithSystemFont("0", "Arial", 48);
	gameScore->setPosition(visibleSize.width / 2, visibleSize.height - 48);
	this->addChild(gameScore);
	score = 0;
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("revolution.mp3", true);
	backgroundMusicID = AudioEngine::play2d("revolution.mp3");
	AudioEngine::setVolume(backgroundMusicID, 0.5);
	particleSystem = ParticleSystemQuad::create("falling-dust.plist");
	//particleSystem->setBlendAdditive(true);
	particleSystem->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 250));
	this->addChild(particleSystem, 10);
	//log("create!");
	/*settingOption = Sprite::create("../Resources/setting.png");
	settingOption->setPosition(Point(visibleSize.width - 30, visibleSize.height - 60));
	this->addChild(settingOption, 100);
	EventListenerTouchOneByOne* settingListener = EventListenerTouchOneByOne::create();
	settingListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	settingListener->onTouchEnded = [=](Touch* touch, Event* event) {
		gamePause();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(settingListener, settingOption);*/
	MenuItemImage* settingOption = MenuItemImage::create(
		"setting.png",
		"setting.png",
		this,
		menu_selector(gameScene::gamePause)
	);
	MenuItemImage* quitOption = MenuItemImage::create(
		"quit.png",
		"quit.png",
		this,
		menu_selector(gameScene::gameQuit)
	);
	settingOption->setPosition(Point(visibleSize.width - 30, visibleSize.height - 60));
	quitOption->setPosition(Point(visibleSize.width - 30, visibleSize.height - 180));
	Menu* menu = Menu::create(settingOption, quitOption, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 100);
	this->schedule(schedule_selector(gameScene::volumeListener), 0.1f);
}

void gameScene::gameQuit(Object* pSender)
{
	Size visibleSize = Director::sharedDirector()->getVisibleSize();
	RenderTexture* renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();
	Director::sharedDirector()->pushScene(GameQuit::scene(renderTexture));
}

void gameScene::gamePause(Object* pSender) {
	//log("PauseBegan");
	Size visibleSize = Director::sharedDirector()->getVisibleSize();
	RenderTexture* renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
	//遍历当前类的所有子节点信息，画入renderTexture中。
	//这里类似截图。
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();
	//将游戏界面暂停，压入场景堆栈。并切换到GamePause界面
	Director::sharedDirector()->pushScene(GamePause::scene(renderTexture));
	//log("PauseEnded");
}

void gameScene::volumeListener(float dt) {
	volume = GamePause::getVolumeFloat();
	//log("volume = %f", volume);
	AudioEngine::setVolume(backgroundMusicID, volume);
}

void gameScene::TouchesBegan(Touch* touch, Event* event) {
	gameBackground->addRipple(Point(touch->getLocation().x, touch->getLocation().y), 1);
	if (perfectRect.containsPoint(ccp(touch->getLocation().x, touch->getLocation().y))) {
		for (auto& i : Notes) {
			//log("i=%d", i->bornX);
			if (perfectRect.containsPoint(i->getTagPosition()) && i->noteState==alive) {
				//log("x=%f y=%f", i->getTagPosition().x, i->getTagPosition().y);
				//log("xx=%f yy=%f", touch->getLocation().x, touch->getLocation().y);
				Size visibleSize = Director::getInstance()->getVisibleSize();
				Rect LRRect;
				LRRect=CCRectMake(i->bornX - 40, visibleSize.height / 2 - 275, 80, 50);
				if (LRRect.containsPoint(ccp(touch->getLocation().x, touch->getLocation().y))) {
					//log("perfect!");
					gameScene::newScore(2);
					gameScene::noteRemove(i,1);
					perfectNum++;
					calCombo(i->getTagPosition(),0);
				}
			}
		}
	}else if (greatRect.containsPoint(ccp(touch->getLocation().x, touch->getLocation().y))) {
		for (auto& i : Notes) {
			if (greatRect.containsPoint(i->getTagPosition()) && i->noteState == alive) {
				//log("x=%f y=%f", i->getTagPosition().x, i->getTagPosition().y);
				//log("xx=%f yy=%f", touch->getLocation().x, touch->getLocation().y);
				Size visibleSize = Director::getInstance()->getVisibleSize();
				Rect LRRect;
				LRRect = CCRectMake(i->bornX - 40, visibleSize.height / 2 - 300, 80, 100);
				if (LRRect.containsPoint(ccp(touch->getLocation().x, touch->getLocation().y))) {
					//log("great!");
					gameScene::newScore(1);
					gameScene::noteRemove(i,1);
					greatNum++;
					calCombo(i->getTagPosition(),0);
				}
			}
		}
	}
}

void gameScene::calCombo(Point curPos,bool stopped) {
	if (!stopped) {
		comboNum++;
		if (comboNum >= 3) {
			Sprite* showCombo = Sprite::create("../Resources/Combo.png");
			Size visibleSize = Director::getInstance()->getVisibleSize();
			showCombo->setPosition(Point(curPos.x+60,curPos.y-20));
			showCombo->setCascadeOpacityEnabled(true);
			ActionInterval* forwardOut = FadeOut::create(0.5f);
			ActionInterval* forwardIn = FadeIn::create(0.5f);
			Action* action = Sequence::create(forwardIn, forwardOut, NULL);
			this->addChild(showCombo,99);
			showCombo->runAction(action);
		}
	}
	else {
		comboNum = 0;
	}

}

void gameScene::TouchesEnded(Touch* touch,Event* event) {
	
}

void gameScene::newScore(int addiScore) {
	score+=addiScore;
	String* content = String::createWithFormat("%d", score);
	const char* s = content->getCString();
	//log("score=%d", score);
	gameScore->setString(s);
}