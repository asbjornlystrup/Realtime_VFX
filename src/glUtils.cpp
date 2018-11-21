#include "glUtils.h"

void printGlError() {
	GLenum error = glGetError();

	if (error != GL_NO_ERROR) {
		switch (error) {
		case GL_INVALID_ENUM:
			throw std::exception("GL error: GL_INVALID_ENUM\n");
			break;

		case GL_INVALID_VALUE:
			throw std::exception("GL error: GL_INVALID_VALUE\n");
			break;

		case GL_INVALID_OPERATION:
			throw std::exception("GL error: GL_INVALID_OPERATION\n");
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			throw std::exception("GL error: GL_INVALID_FRAMEBUFFER_OPERATION\n");
			break;

		case GL_OUT_OF_MEMORY:
			throw std::exception("GL error: GL_OUT_OF_MEMORY\n");
			break;

		case GL_STACK_UNDERFLOW:
			throw std::exception("GL error: GL_STACK_UNDERFLOW\n");
			break;

		case GL_STACK_OVERFLOW:
			throw std::exception("GL error: GL_STACK_OVERFLOW\n");
			break;

		default:
			throw std::exception("GL error: Unknown error\n");
		}
	}
}