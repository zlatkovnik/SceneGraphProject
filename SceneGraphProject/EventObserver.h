#pragma once

class GLFWwindow;
class EventObserver
{
public:
	virtual void MouseMove(double xoffset, double yoffset) {};
	virtual void KeyDown(GLFWwindow* window) {};
	virtual void KeyPress(int key, int action) {};
	virtual void Scroll(double yoffset) {};
};

