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
bool GamePause::init()
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
	volumeController->setMinimumValue(0);         //设置滑块最小值
	volumeController->setMaximumValue(1);       //设置滑块最大值
	volumeController->setMinimumAllowedValue(0); //设置允许滑动的最小值
	volumeController->setMaximumAllowedValue(1); //设置允许滑动的最大值
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
