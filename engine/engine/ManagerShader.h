#pragma once
#include "Singleton.h"
#include <map>

class VSShaderLib;
class ManagerShader {
	SINGLETON_HEADER(ManagerShader)
public:
	char* name;
	std::map<char*, VSShaderLib*> shaderPrograms;		//contains ids of all uniforms

	void add(char* name, VSShaderLib* material);
	VSShaderLib* get(char* name);

	/*
	* Debugger
	*/
	void flushManagerMesh();
private:
	std::map<char*, VSShaderLib*>::iterator _shaderProgramsIterator;
};

