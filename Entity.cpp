#include"Entity.h"
#include<cocos2d.h>
using namespace cocos2d;
void Entity::bindSprite(Sprite* sprite) {
	m_sprite = sprite;
	this->addChild(m_sprite);
}
void Entity::setTagPosition(int x, int y) {
	setPosition(Point(x, y));
}
Point Entity::getTagPosition() {
	return getPosition();
}
void Entity::setController(Controllers* controller) {
	this->m_controller = controller;
	m_controller->setControllerListener(this);
}