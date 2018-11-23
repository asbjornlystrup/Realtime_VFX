#include "image.h"

Image::Image(std::string filePath) {
	m_surface = FreeImage_Load(FIF_PNG, filePath.c_str());
	FreeImage_FlipVertical(m_surface);

	assert(m_surface != nullptr);
}

Image::Image(glm::ivec2 size, int bytesPerPixel, unsigned char *pixelData) {
	m_surface = FreeImage_ConvertFromRawBits(pixelData, size.x, size.y, bytesPerPixel * 8, bytesPerPixel * size.x, 0x000000ff, 0x0000ff00, 0x00ff0000);
	// SDL version: m_surface = SDL_CreateRGBSurfaceFrom((void*)pixelData, size.x, size.y, bytesPerPixel * 8, bytesPerPixel * size.x, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	assert(m_surface != nullptr);
}

Image::~Image() {
	FreeImage_Unload(m_surface);
}

glm::ivec2 Image::getSize() {
	return glm::ivec2(FreeImage_GetWidth(m_surface), FreeImage_GetHeight(m_surface));
}

unsigned char *Image::getPixelData() {
	return (unsigned char*)(FreeImage_GetBits(m_surface));
}

void Image::save(std::string filePath) {
	FreeImage_Save(FIF_PNG, m_surface, filePath.c_str());
}