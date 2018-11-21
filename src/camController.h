#pragma once

#include "config.h"
#include "cam.h"

class Window;

class CamController {
public:
	CamController(Window *window, Cam *cam);
	void tick();
	void keyCallback(int key, int scancode, int action, int mods);
	void cursorPosCallback(double x, double y);

private:
	bool m_keyHeld[1024];
	Window *m_window;
	Cam *m_cam;
	float m_speed;
	float m_mouseSensitivity;
};