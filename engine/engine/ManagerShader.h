#pragma once
#include "Singleton.h"
#include <map>

class ShaderProgram;
class ManagerShader {
	SINGLETON_HEADER(ManagerShader)
public:
	char* name;
	std::map<char*, ShaderProgram*> shaderPrograms;		//contains ids of all uniforms

	void add(char* name, ShaderProgram* material);
	ShaderProgram* get(char* name);

	/*
	* Debugger
	*/
	void flushManagerMesh();
private:
	std::map<char*, ShaderProgram*>::iterator _shaderProgramsIterator;
};

