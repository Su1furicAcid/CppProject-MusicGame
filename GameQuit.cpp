#include"GameQuit.h"
#include"cocos2d.h"
#include"cocos-ext.h"
#include "AudioEngine.h"
#include "mainScene.h"
using namespace experimental;
using namespace cocos2d;
using namespace cocos2d::extension;

Scene* GameQuit::scene(RenderTexture* sqr) {
	Scene* scene = Scene::create();
	GameQuit* layer = GameQuit::create();
	scene->addChild(layer, 1);
	//增加部分：使用Game界面中截图的sqr纹理图片创建Sprite
	//并将Sprite添加到GamePause场景层中
	//得到窗口的大小
	Size visibleSize = Director::sharedDirector()->getVisibleSize();
	Sprite* back_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
	back_spr->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2)); //放置位置,这个相对于中心位置。
	back_spr->setFlipY(true);            //翻转，因为UI坐标和OpenGL坐标不同
	back_spr->setColor(Color3B::GRAY); //图片颜色变灰色
	scene->addChild(back_spr);
	return scene;
}
bool GameQuit::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//得到窗口的大小
	Size visibleSize = Director::sharedDirector()->getVisibleSize();
	//原点坐标
	Point origin = Director::sharedDirector()->getVisibleOrigin();
	//继续游戏按钮
	MenuItemImage* pContinueItem = MenuItemImage::create(
		"pause_continue.png",
		"pause_continue.png",
		this,
		menu_selector(GameQuit::menuContinueCallback)
	);
	MenuItemImage* pExitItem = MenuItemImage::create(
		"Exit.png",
		"Exit.png",
		this,
		menu_selector(GameQuit::quitGame)
	);
	pContinueItem->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2 - 50));
	pExitItem->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2 + 50));
	CCMenu* pMenu = CCMenu::create(pContinueItem, pExitItem, NULL);
	pMenu->setPosition(Point::ZERO);
	this->addChild(pMenu, 2);
	AudioEngine::pauseAll();
	return true;
}
void GameQuit::menuContinueCallback(Object* pSender)
{
	Director::sharedDirector()->popScene();
	AudioEngine::resumeAll();
}
void GameQuit::quitGame(Object* pSender)
{
	auto scene = mainScene::createScene();
	TransitionFade* trans = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(trans);
}