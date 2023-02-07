#pragma once
#include<cocos2d.h>
#define maxnum 100010
#include<vector>
using namespace cocos2d;
class CallbackTimeCounter :public Node {
public:
	CREATE_FUNC(CallbackTimeCounter);
	virtual bool init();
	virtual void update(float dt);

	void start(std::vector<float> fCBtime, std::function<void()> func);
private:
	float m_fTime;
	std::vector<float> m_CBtime;
	bool m_isCounting;
	int dfn;

	std::function<void()> m_func;
};