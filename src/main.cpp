#include "config.h"
#include "window.h"
#include "prerenderUtils.h"

#undef main

int main(void) {
	FreeImage_Initialise();

	// Prerendering requires raw 3ds Max images in the graphics directory
	bool prerender = false;

	if (prerender) {
		std::string directory = "../graphics/flipbookAnims/explosion/";
		prerenderFlipbookMaps(directory, 50);
	}
	
	Window *window = new Window();

	window->mainLoop();

	delete window;

	FreeImage_DeInitialise();

	exit(EXIT_SUCCESS);
}