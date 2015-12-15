#pragma once
#include "ManagerMaterial.h"
#include <iostream>

SINGLETON_IMPLEMENTATION_NO_CONSTRUCTOR(ManagerMaterial)

ManagerMaterial::ManagerMaterial() {}

void ManagerMaterial::add(char* name, Material* material) {
	materials.insert(std::pair<char*, Material*>(name, material));
}

Material* ManagerMaterial::get(char* name) {
	try {
		return materials.find(name)->second;
	} catch (std::exception) {
		std::cout << "\nManagerMaterial::get::Element not found::" << name;
	}
	std::cout << "\nManagerMaterial::get::Element not found::" << name;
	return nullptr;
}

void ManagerMaterial::flushManagerMesh()
{
	std::cout << "\nManagerMaterial::flush::";
	for (_materialsIterator = materials.begin(); _materialsIterator != materials.end(); ++_materialsIterator) {
		std::cout << "\nElement::" << _materialsIterator->first;
	}
}


