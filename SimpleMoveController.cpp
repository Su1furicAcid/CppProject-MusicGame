#include"cocos2d.h"
#include"SimpleMoveController.h"
using namespace cocos2d;
bool SimpleMoveController::init() {
	this->m_iSpeed = 0;
	this->scheduleUpdate();
	return true;
}
void SimpleMoveController::update(float dt) {
	if (m_controllerListener == NULL) {
		return;
	}
	Point pos = m_controllerListener->getTagPosition();
	pos.y += m_iSpeed;
	m_controllerListener->setTagPosition(pos.x, pos.y);
}
void SimpleMoveController::setiSpeed(int iSpeed) {
	this->m_iSpeed = iSpeed;
}