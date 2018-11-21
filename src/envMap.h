#pragma once

#include "config.h"
#include "image.h"

class EnvMap {
public:
	EnvMap(std::string directory);
	~EnvMap();
	GLuint getTextureID();

private:
	GLuint m_textureID;
};