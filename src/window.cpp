#include "window.h"
#include "scene.h"
#include "camController.h"

Window *Window::s_this = nullptr;

Window::Window() {
	assert(s_this == nullptr);
	s_this = this;

	// Initialize GLFW
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW.\n");
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	m_size = glm::ivec2(1280, 720);
	std::string windowTitle = "CSE 163 - Assignment 3";
	m_glfwWindow = glfwCreateWindow(m_size.x, m_size.y, windowTitle.c_str(), nullptr, nullptr);
	setCursorPos((glm::dvec2)m_size / 2.0);

	// Check if the window could not be created
	if (!m_glfwWindow) {
		throw std::runtime_error("Failed to open GLFW window.\n"
				"Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
	}

	// Make the context of the window
	glfwMakeContextCurrent(m_glfwWindow);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(m_glfwWindow, &m_size.x, &m_size.y);

	setupGlew();

	// Print OpenGL and GLSL versions
	printVersions();

	GLint max3dTextureSize;
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max3dTextureSize);
	printf("Max 3D texture size: %d\n", max3dTextureSize);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	m_scene = new Scene(this);
	m_camController = new CamController(this, m_scene->getCam());
	m_tickCounter = 0;

	setupCallbacks();

	// Call the resize callback to make sure things get drawn immediately
	resizeCallback(m_glfwWindow, m_size.x, m_size.y);

	for (int i = 0; i < 1024; i++) {
		m_keyHeld[i] = false;
	}
}

Window::~Window() {
	glfwDestroyWindow(m_glfwWindow);
	glfwTerminate();

	delete m_scene;
	delete m_camController;
}

void Window::mainLoop() {
	// Loop while GLFW window should stay open
	while (!glfwWindowShouldClose(m_glfwWindow)) {
		draw();
		tick();
	}
}

void Window::draw() {
	m_scene->draw();

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	glfwSwapBuffers(m_glfwWindow);
}

void Window::tick() {
	m_camController->tick();

	if (m_keyHeld[GLFW_KEY_1]) {
		DirectionalLight *directionalLight = m_scene->getDirectionalLight();
		float angle = m_dirLightRotCounter * 0.01f;
		directionalLight->direction = glm::normalize(glm::vec3(glm::cos(angle), -0.5, glm::sin(angle)));
		m_dirLightRotCounter++;
	}

	if (!m_keyHeld[GLFW_KEY_2]) {
		std::vector<FlipbookParticle*> flipbookParticles = m_scene->getFlipbookParticles();

		for (int i = 0; i < flipbookParticles.size(); i++) {
			flipbookParticles[i]->tick();
		}
	}

	m_tickCounter++;
}

glm::ivec2 Window::getSize() {
	return m_size;
}

glm::dvec2 Window::getCursorPos() {
	glm::dvec2 cursorPos;
	glfwGetCursorPos(m_glfwWindow, &cursorPos.x, &cursorPos.y);
	return cursorPos;
}

void Window::setCursorPos(glm::dvec2 pos) {
	glfwSetCursorPos(m_glfwWindow, pos.x, pos.y);
}

void Window::keyCallback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods) {
	s_this->m_camController->keyCallback(key, scancode, action, mods);

	if (action == GLFW_PRESS) {
		s_this->m_keyHeld[key] = true;
	}

	if (action == GLFW_RELEASE) {
		s_this->m_keyHeld[key] = false;
	}

	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE: {
				glfwSetWindowShouldClose(s_this->m_glfwWindow, GL_TRUE);
				break;
			}

			case GLFW_KEY_3: {
				auto pointLight = s_this->m_scene->getPointLight();
				auto dirLight = s_this->m_scene->getDirectionalLight();
				if (pointLight->enabled && dirLight->enabled) {
					dirLight->enabled = true;
					pointLight->enabled = false;
				}
				else if (!pointLight->enabled && dirLight->enabled) {
					dirLight->enabled = false;
					pointLight->enabled = true;
				}
				else if (pointLight->enabled && !dirLight->enabled) {
					dirLight->enabled = true;
					pointLight->enabled = true;
				}
				break;
			}

			case GLFW_KEY_4: {
				auto anim = s_this->m_scene->getExplosionAnim();
				anim->toggleSpecular();
				break;
			}
		}
	}
}

void Window::scrollCallback(GLFWwindow *glfwWindow, double xOffset, double yOffset) {
}

void Window::cursorPosCallback(GLFWwindow *glfwWindow, double x, double y) {
	s_this->m_camController->cursorPosCallback(x, y);
}

void Window::mouseButtonCallback(GLFWwindow *glfwWindow, int button, int action, int mods) {
}

void Window::resizeCallback(GLFWwindow *glfwWindow, int newWidth, int newHeight) {
	s_this->setCursorPos(glm::dvec2(newWidth, newHeight) / 2.0);
	s_this->m_scene->resizeCallback(glm::ivec2(newWidth, newHeight));
}

void Window::errorCallback(int error, const char *description) {
	// Print error
	throw std::runtime_error(description);
}

void Window::setupCallbacks() {
	glfwSetErrorCallback(Window::errorCallback);
	glfwSetFramebufferSizeCallback(m_glfwWindow, Window::resizeCallback);
	glfwSetKeyCallback(m_glfwWindow, Window::keyCallback);

	glfwSetCursorPosCallback(m_glfwWindow, Window::cursorPosCallback);
	glfwSetMouseButtonCallback(m_glfwWindow, Window::mouseButtonCallback);
	glfwSetScrollCallback(m_glfwWindow, Window::scrollCallback);
}

void Window::setupGlew() {
	// Initialize GLEW

	GLenum err = glewInit();

	if (err != GLEW_OK) {
		// Problem: glewInit failed, something is seriously wrong.
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
	}

	fprintf(stdout, "Current GLEW version: %s\n", glewGetString(GLEW_VERSION));
}

void Window::printVersions() {
	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Supported OpenGL version: %s\n", glGetString(GL_VERSION));

	// If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
	printf("Supported GLSL version: %s.\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
}