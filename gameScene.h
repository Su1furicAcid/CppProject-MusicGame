#pragma once
#include"cocos2d.h"
#include"SimpleAudioEngine.h"
#include"Player.h"
#include"Entity.h"
#include"SimpleMoveController.h"
#include<vector>
#include"PhysicsRippleSprite.h"
#include<vector>
using namespace cocos2d;
class gameScene :public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(gameScene);
private:
	void initBG();
private:
	PhysicsRippleSprite* gameBackground;
public:
	//音乐播放时间
	int mp3Seconds;
	//当前音乐播放时间
	int currentSeconds;
	//生命血条
	int HP;
	int lastHP;
	//音符出生坐标
	Point bornNotePos;
	//按键监听范围
	Rect keyRect;
	Rect perfectRect;
	Rect greatRect;
	//Perfect击中数量
	//Great击中数量
	//miss数量
	//得分
	int perfectNum;
	int greatNum;
	int missNum;
	int score;
	Label* gameScore;
	void noteBorn();
	void timeCounter();
	void noteRemove(Player* pnote,int Type);
	void noteMoving(float dt);
	void createUI();
	void TouchesBegan(Touch* touch, Event* event);
	void TouchesEnded(Touch* touch,Event* event);
	Vector<Player*> Notes;
	Vector<SimpleMoveController*> MoveControllers;
	void noteLifeListener(float dt);
	void newScore(int addiScore);
	void calCombo(Point curPos,bool stopped);
	int comboNum;
	void loadData();
	std::vector<float> timePointArray;
	ParticleSystemQuad* particleSystem;
	int backgroundMusicID;
	int failEffectID;
	int successEffectID;
	//Sprite* settingOption;
	void gamePause(Object* pSender);
	float volume;
	void volumeListener(float dt);
	void gameQuit(Object* pSender);
};