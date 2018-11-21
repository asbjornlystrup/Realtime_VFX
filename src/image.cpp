#include "image.h"

Image::Image(std::string filePath) {
	m_surface = IMG_Load(filePath.c_str());
	assert(m_surface != nullptr);
}

Image::Image(glm::ivec2 size, int bytesPerPixel, unsigned char *pixelData) {
	m_surface = SDL_CreateRGBSurfaceFrom((void*)pixelData, size.x, size.y, bytesPerPixel * 8, bytesPerPixel * size.x, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	assert(m_surface != nullptr);
}

Image::~Image() {
	SDL_FreeSurface(m_surface);
}

glm::ivec2 Image::getSize() {
	return glm::ivec2(m_surface->w, m_surface->h);
}

unsigned char *Image::getPixelData() {
	return (unsigned char*)(m_surface->pixels);
}

void Image::save(std::string filePath) {
	IMG_SavePNG(m_surface, filePath.c_str());
}