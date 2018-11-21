#include "config.h"
#include "window.h"
#include "prerenderUtils.h"

#undef main

int main(void) {
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::string error = "SDL_image could not initialize! SDL_image error: ";
		error += IMG_GetError();
		throw std::exception(error.c_str());
	}

	bool prerender = false;

	if (prerender) {
		std::string directory = "../graphics/flipbookAnims/explosion/";
		prerenderFlipbookMaps(directory, 50);
	}
	
	Window *window = new Window();

	window->mainLoop();

	delete window;

	exit(EXIT_SUCCESS);
}