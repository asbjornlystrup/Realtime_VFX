#pragma once

#include "config.h"
#include "mesh.h"
#include "image.h"
#include "envMap.h"
#include "flipbookAnim.h"

class FlipbookParticle {
public:
	FlipbookParticle(FlipbookAnim* flipbookAnim, glm::vec3 pos, glm::vec2 size, EnvMap *envMap, int startKeyframe);
	~FlipbookParticle();
	void draw();
	void tick();
	glm::vec3 getPos();

private:
	glm::vec3 m_pos;
	glm::vec2 m_size;
	glm::mat4 m_worldMat;
	int m_currKeyframe;
	int m_keyframeCount;
	EnvMap *m_envMap;
	float m_blendFactor;
	FlipbookAnim *m_flipbookAnim;

	void updateWorldMat();
};