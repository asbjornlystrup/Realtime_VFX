#include "envMap.h"

EnvMap::EnvMap(std::string directory) {
	std::string filenames[] = {"right.png", "left.png", "top.png", "base.png", "front.png", "back.png"};

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	for (int i = 0; i < 6; i++) {
		Image *image = new Image(directory + filenames[i]);

		assert(image->getPixelData() != nullptr);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 3, image->getSize().x,
				image->getSize().y, 0, GL_BGR, GL_UNSIGNED_BYTE, image->getPixelData());

		delete image;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

EnvMap::~EnvMap() {
	glDeleteTextures(1, &m_textureID);
}

GLuint EnvMap::getTextureID() {
	return m_textureID;
}