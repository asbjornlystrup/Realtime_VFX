#pragma once

#include "config.h"
#include "envMap.h"
#include "mesh.h"

class Skybox {
public:
	Skybox(EnvMap *envMap);
	~Skybox();
	void draw();
	EnvMap *getEnvMap();

private:
	EnvMap *m_envMap;
	Mesh *m_mesh;
};