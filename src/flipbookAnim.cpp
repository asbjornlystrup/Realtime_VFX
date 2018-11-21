#include "flipbookAnim.h"

FlipbookAnim::FlipbookAnim(std::string directory, int keyframeCount) {
	m_mesh = new Mesh(false);
	m_keyframeCount = keyframeCount;

	m_kDiffuse = glm::vec3(1);
	m_kSpecular = glm::vec3(0);
	m_specularExponent = 16.0f;

	m_mesh->addQuad(glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0));

	m_mesh->updateGlBuffers();
	
	m_normalMapTexIDs.resize(m_keyframeCount);
	glGenTextures(m_normalMapTexIDs.size(), &m_normalMapTexIDs[0]);

	for (int i = 0; i < m_normalMapTexIDs.size(); i++) {
		Image *image = new Image(directory + "normalMap" + std::to_string(i) + ".png");
		
		glBindTexture(GL_TEXTURE_2D, m_normalMapTexIDs[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, 4, image->getSize().x, 
					image->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getPixelData());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		delete image;
	}

	m_diffuseAlphaMapTexIDs.resize(m_keyframeCount);
	glGenTextures(m_diffuseAlphaMapTexIDs.size(), &m_diffuseAlphaMapTexIDs[0]);

	for (int i = 0; i < m_diffuseAlphaMapTexIDs.size(); i++) {
		Image *image = new Image(directory + "diffuseAlphaMap" + std::to_string(i) + ".png");

		glBindTexture(GL_TEXTURE_2D, m_diffuseAlphaMapTexIDs[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, 4, image->getSize().x,
					image->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getPixelData());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		delete image;
	}

	m_opticalFlowMapTexIDs.resize(m_keyframeCount);
	glGenTextures(m_opticalFlowMapTexIDs.size(), &m_opticalFlowMapTexIDs[0]);

	for (int i = 0; i < m_opticalFlowMapTexIDs.size(); i++) {
		Image *image = new Image(directory + "opticalFlowMap" + std::to_string(i) + ".png");

		m_opticalFlowMapSize = image->getSize();

		glBindTexture(GL_TEXTURE_2D, m_opticalFlowMapTexIDs[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, 4, image->getSize().x,
			image->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getPixelData());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		delete image;
	}

	m_distortionStrength = 8.852f / (glm::vec2)m_opticalFlowMapSize;
}

int FlipbookAnim::getKeyframeCount() {
	return m_keyframeCount;
}

void FlipbookAnim::toggleSpecular() {
	m_specular = !m_specular;
	if (!m_specular) {
		m_kDiffuse = glm::vec3(1);
		m_kSpecular = glm::vec3(0);
	}
	else {
		m_kDiffuse = glm::vec3(0.5);
		m_kSpecular = glm::vec3(0.5);
	}
}

FlipbookAnim::~FlipbookAnim() {
	delete m_mesh;
	glDeleteTextures(m_normalMapTexIDs.size(), &m_normalMapTexIDs[0]);
	glDeleteTextures(m_diffuseAlphaMapTexIDs.size(), &m_diffuseAlphaMapTexIDs[0]);
	glDeleteTextures(m_opticalFlowMapTexIDs.size(), &m_opticalFlowMapTexIDs[0]);
}

void FlipbookAnim::draw(int keyframe, float blendFactor, glm::mat4 worldMat, EnvMap* envMap) {
	GLint currShader;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currShader);

	int nextKeyframe = (keyframe + 1) % m_keyframeCount;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_normalMapTexIDs[keyframe]);
	glUniform1i(glGetUniformLocation(currShader, "texNormalMapCurr"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_diffuseAlphaMapTexIDs[keyframe]);
	glUniform1i(glGetUniformLocation(currShader, "texDiffuseAlphaMapCurr"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_opticalFlowMapTexIDs[keyframe]);
	glUniform1i(glGetUniformLocation(currShader, "texOpticalFlowMapCurr"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_normalMapTexIDs[nextKeyframe]);
	glUniform1i(glGetUniformLocation(currShader, "texNormalMapNext"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_diffuseAlphaMapTexIDs[nextKeyframe]);
	glUniform1i(glGetUniformLocation(currShader, "texDiffuseAlphaMapNext"), 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_opticalFlowMapTexIDs[nextKeyframe]);
	glUniform1i(glGetUniformLocation(currShader, "texOpticalFlowMapNext"), 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envMap->getTextureID());
	glUniform1i(glGetUniformLocation(currShader, "texEnvMap"), 6);
	
		glUniformMatrix4fv(glGetUniformLocation(currShader, "worldMat"), 1, GL_FALSE, &worldMat[0][0]);
		glUniform1f(glGetUniformLocation(currShader, "blendFactor"), blendFactor);
		glUniform3fv(glGetUniformLocation(currShader, "kDiffuse"), 1, &m_kDiffuse[0]);
		glUniform3fv(glGetUniformLocation(currShader, "kSpecular"), 1, &m_kSpecular[0]);
		glUniform1f(glGetUniformLocation(currShader, "specularExponent"), m_specularExponent);
		glUniform2fv(glGetUniformLocation(currShader, "distortionStrength"), 1, &m_distortionStrength[0]);
		m_mesh->draw();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}