#include"GamePause.h"
#include"cocos2d.h"
#include"cocos-ext.h"
#include "AudioEngine.h"
using namespace experimental;
using namespace cocos2d;
using namespace cocos2d::extension;

float GamePause::volumeFloat = 0.5;

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

	Label* volumeControlText = Label::create("Volume", "Arial", 20);
	volumeControlText->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 30));
	this->addChild(volumeControlText);

	volumeController = ControlSlider::create(
		"slider_before.png",
		"slider_after.png",
		"slider_btn.png"
	);
	volumeController->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 60));
	this->addChild(volumeController);
	volumeController->setMinimumValue(0);         //���û�����Сֵ
	volumeController->setMaximumValue(1);       //���û������ֵ
	volumeController->setMinimumAllowedValue(0); //��������������Сֵ
	volumeController->setMaximumAllowedValue(1); //���������������ֵ
	volumeController->setValue(volumeFloat);
	volumeController->addTargetWithActionForControlEvents(this, cccontrol_selector(GamePause::volumeChanged), Control::EventType::VALUE_CHANGED);

	AudioEngine::pauseAll();
	log("PausePending");
	return true;
}
void GamePause::menuContinueCallback(Object* pSender)
{
	Director::sharedDirector()->popScene();
	AudioEngine::resumeAll();
}
void GamePause::volumeChanged(Object* pSender, Control::EventType controlEvent)
{
	ControlSlider* slider = (ControlSlider*)pSender;
	volumeFloat = slider->getValue();
	//log("Volume = %f", volumeFloat);
}
float GamePause::getVolumeFloat() {
	return volumeFloat;
}
