#pragma once

#include <string>
#include <iostream>
#include "Singleton.h"
#include "Dependencies/glew/glew.h"

class ManagerOpenGLErrors
{
	SINGLETON_HEADER(ManagerOpenGLErrors)
public:
	void CheckError(std::string error);

private:
	bool isOpenGLError();

};


