#pragma once
#include"ControllerListener.h"
class Controllers :public Node {
public:
	void setControllerListener(ControllerListener* controllerListener);
protected:
	ControllerListener* m_controllerListener;
};