#pragma once

#include "config.h"
#include "mesh.h"
#include "image.h"
#include "envMap.h"

class FlipbookAnim {
public:
	FlipbookAnim(std::string directory, int keyframeCount);
	~FlipbookAnim();
	void draw(int keyframe, float blendFactor, glm::mat4 worldMat, EnvMap *envMap);
	int getKeyframeCount();
	void toggleSpecular();

private:
	Mesh *m_mesh;
	bool m_specular = false;
	std::vector<GLuint> m_normalMapTexIDs;
	std::vector<GLuint> m_diffuseAlphaMapTexIDs;
	std::vector<GLuint> m_opticalFlowMapTexIDs;
	glm::vec3 m_kDiffuse;
	glm::vec3 m_kSpecular;
	float m_specularExponent;
	int m_keyframeCount;
	glm::ivec2 m_opticalFlowMapSize;
	glm::vec2 m_distortionStrength;
};