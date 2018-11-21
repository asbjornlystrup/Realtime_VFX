#include "FlipbookParticle.h"

FlipbookParticle::FlipbookParticle(FlipbookAnim* flipbookAnim, glm::vec3 pos, glm::vec2 size, EnvMap *envMap, int startKeyframe) {
	m_flipbookAnim = flipbookAnim;
	m_pos = pos;
	m_size = size;
	m_envMap = envMap;
	
	m_currKeyframe = startKeyframe;
	m_keyframeCount = flipbookAnim->getKeyframeCount();

	updateWorldMat();

	m_blendFactor = 0.0f;
}

FlipbookParticle::~FlipbookParticle() {
}

void FlipbookParticle::draw() {
	m_flipbookAnim->draw(m_currKeyframe, m_blendFactor, m_worldMat, m_envMap);
}

void FlipbookParticle::tick() {
	m_blendFactor += 0.02f;

	if (m_blendFactor >= 1.0f) {
		m_blendFactor = 0.0f;
		m_currKeyframe = (m_currKeyframe + 1) % m_keyframeCount;
	}
}

void FlipbookParticle::updateWorldMat() {
	m_worldMat = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(m_size / 2.0f, 1.0f)), m_pos);
}

glm::vec3 FlipbookParticle::getPos() {
	return m_pos;
}