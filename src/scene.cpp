#include "scene.h"
#include "window.h"

Scene::Scene(Window *window) {
	m_window = window;
	
	m_skybox = new Skybox(new EnvMap("../graphics/envMaps/space0/"));
	m_cam = new Cam(glm::radians(70.0f), m_window->getSize(), 0.1f, 1000.0f);
	
	m_explosionAnim = new FlipbookAnim("../graphics/flipbookAnims/explosion/", 50);
	m_flipbookParticles.push_back(new FlipbookParticle(m_explosionAnim, glm::vec3(0, 0.25 * 10, -0.5 * 10 - 30), glm::vec2(5, 5), m_skybox->getEnvMap(), 0));
	m_flipbookParticles.push_back(new FlipbookParticle(m_explosionAnim, glm::vec3(0.1 * 10, 0, -0.25 * 10 - 30), glm::vec2(7, 7), m_skybox->getEnvMap(), 10));
	m_flipbookParticles.push_back(new FlipbookParticle(m_explosionAnim, glm::vec3(0, -0.5 * 10, -0.35 * 10 - 30), glm::vec2(5, 5), m_skybox->getEnvMap(), 20));
	m_flipbookParticles.push_back(new FlipbookParticle(m_explosionAnim, glm::vec3(-0.5 * 10, 0.25 * 10, -0.25 * 10 - 30), glm::vec2(5, 5), m_skybox->getEnvMap(), 30));
	m_flipbookParticles.push_back(new FlipbookParticle(m_explosionAnim, glm::vec3(0.35 * 10, 0, -0.5 * 10 - 30), glm::vec2(6, 6), m_skybox->getEnvMap(), 40));
	
	m_ShaderSkybox = loadShaderProg("../shaders/skybox.vert", "../shaders/skybox.frag");
	m_ShaderFlipbook = loadShaderProg("../shaders/flipbook.vert", "../shaders/flipbook.frag");

	m_directionalLight.enabled = 1;
	m_directionalLight.color = glm::vec3(0.8f, 0.9f, 1.0f);
	m_directionalLight.direction = glm::normalize(glm::vec3(1.0, -0.5, 0.0));
	m_directionalLight.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);

	m_pointLight.enabled = 0;
	m_pointLight.color = glm::vec3(1.0f, 0.6f, 1.0f);
	m_pointLight.position = glm::vec3(0.0f, 0.0f, -30.0f);
	m_pointLight.radius = 70.0f;
	m_pointLight.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
}

Scene::~Scene() {
	delete m_skybox;
	delete m_cam;
	delete m_explosionAnim;

	for (int i = 0; i < m_flipbookParticles.size(); i++) {
		delete m_flipbookParticles[i];
	}
}

void Scene::draw() {
	glDepthFunc(GL_ALWAYS);
		glUseProgram(m_ShaderSkybox);
			glUniformMatrix4fv(glGetUniformLocation(m_ShaderSkybox, "projMat"), 1, GL_FALSE, &m_cam->getProjMat(true)[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(m_ShaderSkybox, "viewMat"), 1, GL_FALSE, &m_cam->getViewMat(false)[0][0]);
			m_skybox->draw();
		glUseProgram(0);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
		glUseProgram(m_ShaderFlipbook);
			glUniform1i(glGetUniformLocation(m_ShaderFlipbook, "directionalLight.enabled"), m_directionalLight.enabled);
			glUniform3fv(glGetUniformLocation(m_ShaderFlipbook, "directionalLight.color"), 1, &m_directionalLight.color[0]);
			glUniform3fv(glGetUniformLocation(m_ShaderFlipbook, "directionalLight.direction"), 1, &m_directionalLight.direction[0]);
			glUniform3fv(glGetUniformLocation(m_ShaderFlipbook, "directionalLight.ambientColor"), 1, &m_directionalLight.ambientColor[0]);

			glUniform1i(glGetUniformLocation(m_ShaderFlipbook, "pointLight.enabled"), m_pointLight.enabled);
			glUniform3fv(glGetUniformLocation(m_ShaderFlipbook, "pointLight.color"), 1, &m_pointLight.color[0]);
			glUniform3fv(glGetUniformLocation(m_ShaderFlipbook, "pointLight.position"), 1, &m_pointLight.position[0]);
			glUniform1f(glGetUniformLocation(m_ShaderFlipbook, "pointLight.radius"), m_pointLight.radius);
			glUniform3fv(glGetUniformLocation(m_ShaderFlipbook, "pointLight.ambientColor"), 1, &m_pointLight.ambientColor[0]);
		
			glUniformMatrix4fv(glGetUniformLocation(m_ShaderFlipbook, "projMat"), 1, GL_FALSE, &m_cam->getProjMat()[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(m_ShaderFlipbook, "viewMat"), 1, GL_FALSE, &m_cam->getViewMat()[0][0]);
			glUniform3fv(glGetUniformLocation(m_ShaderFlipbook, "camPos"), 1, &m_cam->getPos()[0]);
			glUniform3fv(glGetUniformLocation(m_ShaderFlipbook, "camUp"), 1, &m_cam->getUpVector()[0]);

			alphaSort();

			for (int i = 0; i < m_flipbookParticles.size(); i++) {
				m_flipbookParticles[i]->draw();
			}
		glUseProgram(0);
	glDisable(GL_BLEND);
}

void Scene::alphaSort() {
	std::vector<float> priorityVec;
	
	glm::vec3 camPos = m_cam->getPos();
	for (int i = 0; i < m_flipbookParticles.size(); i++) {
		glm::vec3 particlePos = m_flipbookParticles[i]->getPos();
		priorityVec.push_back(glm::distance(particlePos, camPos) * 10.0);
	}

	quicksort(priorityVec, m_flipbookParticles, 0, priorityVec.size() - 1);
	
	//printf("\n\n");
	
	/*for (int i = 0; i < priorityVec.size(); i++) {
		printf("%f, ", priorityVec[i]);
	}*/
}

void Scene::swapPriorityVec(std::vector<float> &v, int x, int y) {
	float temp = v[x];
	v[x] = v[y];
	v[y] = temp;
}

void Scene::swapFlipbookParticles(std::vector<FlipbookParticle*> &v, int x, int y) {
	FlipbookParticle* temp = v[x];
	v[x] = v[y];
	v[y] = temp;
}

void Scene::quicksort(std::vector<float> &priorityVec, std::vector<FlipbookParticle*> &flipbookParticles, int L, int R) {
	int i, j, mid;
	float piv;
	i = L;
	j = R;
	mid = L + (R - L) / 2;
	piv = priorityVec[mid];

	while (i < R || j > L) {
		while (priorityVec[i] > piv)
			i++;
		while (priorityVec[j] < piv)
			j--;

		if (i <= j) {
			swapPriorityVec(priorityVec, i, j);
			swapFlipbookParticles(flipbookParticles, i, j);
			i++;
			j--;
		} else {
			if (i < R)
				quicksort(priorityVec, flipbookParticles, i, R);
			if (j > L)
				quicksort(priorityVec, flipbookParticles, L, j);
			return;
		}
	}
}

void Scene::resizeCallback(glm::ivec2 newSize) {
	m_cam->resizeCallback(newSize);
}

Cam *Scene::getCam() {
	return m_cam;
}

DirectionalLight *Scene::getDirectionalLight() {
	return &m_directionalLight;
}

PointLight *Scene::getPointLight() {
	return &m_pointLight;
}

std::vector<FlipbookParticle*> Scene::getFlipbookParticles() {
	return m_flipbookParticles;
}

FlipbookAnim * Scene::getExplosionAnim() {
	return m_explosionAnim;
}
