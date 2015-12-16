#pragma once
#include "ManagerTexture.h"
#include <iostream>

SINGLETON_IMPLEMENTATION_NO_CONSTRUCTOR(ManagerTexture)

ManagerTexture::ManagerTexture() {}

void ManagerTexture::add(char* name, Texture* texture) {
	textures.insert(std::pair<char*, Texture*>(name, texture));
}

Texture* ManagerTexture::get(char* name) {
	try {
		return textures.find(name)->second;
	} catch (std::exception) {
		std::cout << "\nManagerTexture::get::Element not found::" << name;
	}
	std::cout << "\nManagerTexture::get::Element not found::" << name;
	return nullptr;
}

void ManagerTexture::flushManagerMesh() {
	std::cout << "\nManagerTexture::flush::";
	for (_texturesIterator = textures.begin(); _texturesIterator != textures.end(); ++_texturesIterator) {
		std::cout << "\nElement::" << _texturesIterator->first;
	}
}


