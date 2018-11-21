#pragma once

#include "config.h"

class Cam {
public:
	Cam(float fov, glm::ivec2 drawTargetSize, float near, float far);
	void resizeCallback(glm::ivec2 newSize);
	glm::mat4 getProjMat(bool infinite = false);
	glm::mat4 getViewMat(bool includeTrans = true);
	void setPos(glm::vec3 pos);
	glm::vec3 getPos();
	glm::vec3 getUpVector();
	void addPos(glm::vec3 deltaPos);
	void addYaw(float deltaYaw);
	void setPitch(float pitch);
	float getYaw();
	float getPitch();

private:
	glm::mat4 m_normalProjMat;
	glm::mat4 m_infProjMat;
	glm::mat4 m_viewMat;
	float m_fov;
	glm::ivec2 m_drawTargetSize;
	float m_near;
	float m_far;
	glm::vec3 m_pos;
	float m_yaw;
	float m_pitch;
	glm::vec3 m_up;

	void updateProjMats();
	void updateViewMat();
};