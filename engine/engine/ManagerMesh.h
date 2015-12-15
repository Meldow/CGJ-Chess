#pragma once
#include "Singleton.h"
#include <map>

class Mesh;

class ManagerMesh {
	SINGLETON_HEADER(ManagerMesh)
public:
	char* name;
	std::map<char*, Mesh*> meshes;		//contains ids of all uniforms

	void add(char* name, Mesh* mesh);
	Mesh* get(char* name);

	/*
	* Debugger
	*/
	void flushManagerMesh();
private:
	std::map<char*, Mesh*>::iterator _meshesIterator;
};

