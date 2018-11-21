#pragma once

#include "config.h"

class Image {
public:
	Image(std::string filePath);
	Image(glm::ivec2 size, int bytesPerPixel, unsigned char * pixelData);
	~Image();
	unsigned char *getPixelData();
	glm::ivec2 getSize();
	void save(std::string filePath);

private:
	SDL_Surface *m_surface;
};