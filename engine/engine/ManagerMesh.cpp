#pragma once
#include "ManagerMesh.h"
#include <iostream>
#include "Mesh.h"

SINGLETON_IMPLEMENTATION_NO_CONSTRUCTOR(ManagerMesh)

ManagerMesh::ManagerMesh() {}

void ManagerMesh::add(char* name, Mesh* mesh) {
	meshes.insert(std::pair<char*, Mesh*>(name, mesh));
}

Mesh* ManagerMesh::get(char* name) {
	try {
		return meshes.find(name)->second;
	} catch (std::exception) {
		std::cout << "\nManagerMesh::get::Element not found::" << name;
	}
	std::cout << "\nManagerMesh::get::Element not found::" << name;
	return nullptr;
}

void ManagerMesh::flushManagerMesh() {
	std::cout << "\nManagerMesh::flush::";
	for (_meshesIterator = meshes.begin(); _meshesIterator != meshes.end(); ++_meshesIterator) {
		std::cout << "\nElement::" << _meshesIterator->first;
	}
}
