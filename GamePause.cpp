#include"GamePause.h"
#include"cocos2d.h"
using namespace cocos2d;
Scene* GamePause::scene(RenderTexture* sqr) {
	Scene* scene = Scene::create();
	GamePause* layer = GamePause::create();
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
bool GamePause::init()
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
		menu_selector(GamePause::menuContinueCallback));
	pContinueItem->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2 + 30));
	CCMenu* pMenu = CCMenu::create(pContinueItem, NULL);
	pMenu->setPosition(Point::ZERO);
	this->addChild(pMenu, 2);
	return true;
}
void GamePause::menuContinueCallback(Object* pSender)
{
	Director::sharedDirector()->popScene();
}
