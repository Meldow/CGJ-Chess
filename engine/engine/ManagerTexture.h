#pragma once
#include "Singleton.h"
#include <map>

class Texture;

class ManagerTexture {
	SINGLETON_HEADER(ManagerTexture)
public:
	char* name;
	std::map<char*, Texture*> textures;		//contains ids of all uniforms

	void add(char* name, Texture* texture);
	Texture* get(char* name);

	/*
	* Debugger
	*/
	void flushManagerMesh();
private:
	std::map<char*, Texture*>::iterator _texturesIterator;
};

