#include "ManagerOpenGLErrors.h"

SINGLETON_IMPLEMENTATION_NO_CONSTRUCTOR(ManagerOpenGLErrors)

ManagerOpenGLErrors::ManagerOpenGLErrors() {}

void ManagerOpenGLErrors::CheckError(std::string error) {
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		while (true);
		exit(EXIT_FAILURE);
	}
}

bool ManagerOpenGLErrors::isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}