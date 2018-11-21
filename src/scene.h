#pragma once

#include "config.h"
#include "skybox.h"
#include "cam.h"
#include "shaderMethods.h"
#include "flipbookParticle.h"

class Window;

struct DirectionalLight {
	int enabled;
	glm::vec3 color;
	glm::vec3 direction;
	glm::vec3 ambientColor;
};

struct PointLight {
	int enabled;
	glm::vec3 color;
	glm::vec3 position;
	float radius;
	glm::vec3 ambientColor;
};

class Scene {
public:
	Scene(Window *window);
	~Scene();
	void draw();
	void resizeCallback(glm::ivec2 newSize);
	Cam *getCam();
	DirectionalLight *getDirectionalLight();
	PointLight *getPointLight();
	std::vector<FlipbookParticle*> getFlipbookParticles();
	FlipbookAnim* getExplosionAnim();

private:
	Window *m_window;
	Skybox *m_skybox;
	Cam *m_cam;
	GLuint m_ShaderSkybox;
	GLuint m_ShaderFlipbook;
	std::vector<FlipbookParticle*> m_flipbookParticles;
	DirectionalLight m_directionalLight;
	PointLight m_pointLight;
	FlipbookAnim* m_explosionAnim;

	void alphaSort();
	void swapPriorityVec(std::vector<float> &v, int x, int y);
	void swapFlipbookParticles(std::vector<FlipbookParticle*> &v, int x, int y);
	void quicksort(std::vector<float> &priorityVec, std::vector<FlipbookParticle*> &flipbookParticles, int L, int R);
};