#include"CallbackTimeCounter.h"
#include<vector>
using namespace cocos2d;
bool CallbackTimeCounter::init() {
	m_isCounting = false;
	this->scheduleUpdate();
	return true;
}
void CallbackTimeCounter::update(float dt) {
	if (!m_isCounting) {
		return;
	}
	m_fTime += dt;
	if (dfn<m_CBtime.size()) {
		if (m_fTime >= m_CBtime.at(dfn)) {
			m_func();
			dfn++;
			//m_isCounting = false;
		}
	}
}
void CallbackTimeCounter::start(std::vector<float> fCBTime, std::function<void()>func) {
	for (int i = 0; i < fCBTime.size(); i++) {
		m_CBtime.push_back(fCBTime[i]);
	}
	m_fTime = 0;
	dfn = 0;
	m_func = func;
	m_isCounting = true;
}