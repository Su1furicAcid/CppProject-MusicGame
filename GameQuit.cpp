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
	//���Ӳ��֣�ʹ��Game�����н�ͼ��sqr����ͼƬ����Sprite
	//����Sprite��ӵ�GamePause��������
	//�õ����ڵĴ�С
	Size visibleSize = Director::sharedDirector()->getVisibleSize();
	Sprite* back_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
	back_spr->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2)); //����λ��,������������λ�á�
	back_spr->setFlipY(true);            //��ת����ΪUI�����OpenGL���겻ͬ
	back_spr->setColor(Color3B::GRAY); //ͼƬ��ɫ���ɫ
	scene->addChild(back_spr);
	return scene;
}
bool GameQuit::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//�õ����ڵĴ�С
	Size visibleSize = Director::sharedDirector()->getVisibleSize();
	//ԭ������
	Point origin = Director::sharedDirector()->getVisibleOrigin();
	//������Ϸ��ť
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