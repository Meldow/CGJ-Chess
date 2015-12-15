#pragma once
#include "Singleton.h"
#include <map>

class Material;

class ManagerMaterial {
	SINGLETON_HEADER(ManagerMaterial)
public:
	char* name;
	std::map<char*, Material*> materials;		//contains ids of all uniforms

	void add(char* name, Material* material);
	Material* get(char* name);

	/*
	* Debugger
	*/
	void flushManagerMesh();
private:
	std::map<char*, Material*>::iterator _materialsIterator;
};

