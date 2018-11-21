#include "cam.h"

Cam::Cam(float fov, glm::ivec2 drawTargetSize, float near, float far) {
	m_fov = fov;
	m_near = near;
	m_far = far;

	m_pos = glm::vec3(0, 0, 0);
	m_yaw = -PI / 2.0f;
	m_pitch = 0.0f;
	m_up = glm::vec3(0, 1, 0);
	updateViewMat();
	resizeCallback(drawTargetSize);
}

void Cam::resizeCallback(glm::ivec2 newSize) {
	m_drawTargetSize = newSize;
	updateProjMats();
}

glm::mat4 Cam::getProjMat(bool infinite) {
	if (infinite) {
		return m_infProjMat;
	} else {
		return m_normalProjMat;
	}
}

glm::mat4 Cam::getViewMat(bool includeTrans) {
	if (includeTrans) {
		return m_viewMat;
	} else {
		glm::mat4 viewMatNoTrans = m_viewMat;
		viewMatNoTrans[3] = glm::vec4(0, 0, 0, 1);
		return viewMatNoTrans;
	}
}

void Cam::setPos(glm::vec3 pos) {
	m_pos = pos;
	updateViewMat();
}

glm::vec3 Cam::getPos() {
	return m_pos;
}

glm::vec3 Cam::getUpVector() {
	return m_up;
}

void Cam::addPos(glm::vec3 deltaPos) {
	m_pos += deltaPos;
	updateViewMat();
}

void Cam::addYaw(float deltaYaw) {
	m_yaw += deltaYaw;
	updateViewMat();
}

void Cam::setPitch(float pitch) {
	m_pitch = pitch;
	updateViewMat();
}

float Cam::getYaw() {
	return m_yaw;
}

float Cam::getPitch() {
	return m_pitch;
}

void Cam::updateProjMats() {
	m_normalProjMat = glm::perspective(m_fov, (float)m_drawTargetSize.x / (float)m_drawTargetSize.y, m_near, m_far);
	m_infProjMat = glm::infinitePerspective(m_fov, (float)m_drawTargetSize.x / (float)m_drawTargetSize.y, m_near);
}

void Cam::updateViewMat() {
	glm::vec3 center = m_pos + glm::vec3(glm::cos(m_yaw) * glm::cos(m_pitch), glm::sin(m_pitch), glm::sin(m_yaw) * glm::cos(m_pitch));
	m_viewMat = glm::lookAt(m_pos, center, m_up);
}