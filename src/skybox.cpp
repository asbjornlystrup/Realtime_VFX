#include "skybox.h"

Skybox::Skybox(EnvMap *envMap) {
	m_envMap = envMap;

	m_mesh = new Mesh(false);

	float r = 9999999.0f;
	m_mesh->addQuad(glm::vec3(-r, r, -r), glm::vec3(r, r, -r), glm::vec3(-r, -r, -r), glm::vec3(r, -r, -r));
	m_mesh->addQuad(glm::vec3(r, r, -r), glm::vec3(r, r, r), glm::vec3(r, -r, -r), glm::vec3(r, -r, r));
	m_mesh->addQuad(glm::vec3(r, r, r), glm::vec3(-r, r, r), glm::vec3(r, -r, r), glm::vec3(-r, -r, r));
	m_mesh->addQuad(glm::vec3(-r, r, r), glm::vec3(-r, r, -r), glm::vec3(-r, -r, r), glm::vec3(-r, -r, -r));
	m_mesh->addQuad(glm::vec3(-r, r, r), glm::vec3(r, r, r), glm::vec3(-r, r, -r), glm::vec3(r, r, -r));
	m_mesh->addQuad(glm::vec3(-r, -r, -r), glm::vec3(r, -r, -r), glm::vec3(-r, -r, r), glm::vec3(r, -r, r));

	m_mesh->updateGlBuffers();
}

Skybox::~Skybox() {
	delete m_envMap;
	delete m_mesh;
}

void Skybox::draw() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_envMap->getTextureID());
	
		m_mesh->draw();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

EnvMap *Skybox::getEnvMap() {
	return m_envMap;
}