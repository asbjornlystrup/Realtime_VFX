#pragma once

#include "config.h"

class Scene;
class CamController;

class Window {
public:
	Window();
	~Window();
	void mainLoop();
	void draw();
	void tick();
	glm::ivec2 getSize();
	glm::dvec2 getCursorPos();
	void setCursorPos(glm::dvec2 pos);
	static void keyCallback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow *glfwWindow, double xOffset, double yOffset);
	static void cursorPosCallback(GLFWwindow *glfwWindow, double x, double y);
	static void mouseButtonCallback(GLFWwindow *glfwWindow, int button, int action, int mods);
	static void resizeCallback(GLFWwindow *glfwWindow, int newWidth, int newHeight);
	static void errorCallback(int error, const char *description);

private:
	glm::ivec2 m_size;
	Scene *m_scene;
	GLFWwindow *m_glfwWindow;
	CamController *m_camController;
	unsigned int m_tickCounter;
	unsigned int m_dirLightRotCounter;
	bool m_keyHeld[1024];
	static Window *s_this;

	void setupCallbacks();
	void setupGlew();
	void printVersions();
};