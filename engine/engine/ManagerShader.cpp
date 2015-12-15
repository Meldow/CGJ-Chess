#pragma once
#include "ManagerShader.h"
#include <iostream>

SINGLETON_IMPLEMENTATION_NO_CONSTRUCTOR(ManagerShader)

ManagerShader::ManagerShader() {}

void ManagerShader::add(char* name, ShaderProgram* shaderProgram) {
	shaderPrograms.insert(std::pair<char*, ShaderProgram*>(name, shaderProgram));
}

ShaderProgram* ManagerShader::get(char* name) {
	try {
		return shaderPrograms.find(name)->second;
	} catch (std::exception) {
		std::cout << "\nManagerShader::get::Element not found::" << name;
	}
	std::cout << "\nManagerShader::get::Element not found::" << name;
	return nullptr;
}

void ManagerShader::flushManagerMesh() {
	std::cout << "\nManagerShader::flush::";
	for (_shaderProgramsIterator = shaderPrograms.begin(); _shaderProgramsIterator != shaderPrograms.end(); ++_shaderProgramsIterator) {
		std::cout << "\nElement::" << _shaderProgramsIterator->first;
	}
}


