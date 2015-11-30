#pragma once

#include <string>
#include "Singleton.h"

class ManagerOpenGLErrors {
	SINGLETON_HEADER(ManagerOpenGLErrors)
public:
	void CheckError(std::string error);

private:
	bool isOpenGLError();

};


