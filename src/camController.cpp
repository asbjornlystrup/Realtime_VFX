#include "camController.h"
#include "window.h"

CamController::CamController(Window *window, Cam *cam) {
	m_window = window;
	m_cam = cam;

	m_mouseSensitivity = 0.003f;
	m_speed = 0.1f;

	for (int i = 0; i < 1024; i++) {
		m_keyHeld[i] = false;
	}
}

void CamController::tick() {
	float yaw = m_cam->getYaw();

	if (m_keyHeld[GLFW_KEY_W]) {
		m_cam->addPos(glm::vec3(glm::cos(yaw), 0, glm::sin(yaw)) * m_speed);
	}

	if (m_keyHeld[GLFW_KEY_S]) {
		m_cam->addPos(glm::vec3(glm::cos(yaw), 0, glm::sin(yaw)) * -m_speed);
	}

	if (m_keyHeld[GLFW_KEY_A]) {
		m_cam->addPos(glm::vec3(glm::cos(yaw + PI / 2.0f), 0, glm::sin(yaw + PI / 2.0f)) * -m_speed);
	}

	if (m_keyHeld[GLFW_KEY_D]) {
		m_cam->addPos(glm::vec3(glm::cos(yaw + PI / 2.0f), 0, glm::sin(yaw + PI / 2.0f)) * m_speed);
	}

	if (m_keyHeld[GLFW_KEY_SPACE]) {
		m_cam->addPos(glm::vec3(0, m_speed, 0));
	}

	if (m_keyHeld[GLFW_KEY_LEFT_SHIFT]) {
		m_cam->addPos(glm::vec3(0, -m_speed, 0));
	}
}

void CamController::keyCallback(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		m_keyHeld[key] = true;
	}

	if (action == GLFW_RELEASE) {
		m_keyHeld[key] = false;
	}
}

void CamController::cursorPosCallback(double x, double y) {
	glm::dvec2 cursorPos = m_window->getCursorPos();
	float pitch = m_cam->getPitch();

	glm::dvec2 windowCenter = (glm::dvec2)m_window->getSize() / 2.0;
	
	m_cam->addYaw((cursorPos.x - windowCenter.x) * m_mouseSensitivity);
	m_cam->setPitch(glm::clamp(pitch + (float)(windowCenter.y - cursorPos.y) * m_mouseSensitivity, -PI / 2.0f + 0.001f, PI / 2.0f - 0.001f));

	m_window->setCursorPos(windowCenter);
}