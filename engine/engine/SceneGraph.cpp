#pragma once
#include "SceneGraph.h"
#include "SceneNode.h"

SceneGraph::SceneGraph() {}
SceneGraph::~SceneGraph() {}

void SceneGraph::addSceneNode(char* name, SceneNode* scene_node) {
	sceneNodes.insert(std::pair<char*, SceneNode*>(name, scene_node));
	scene_node->name = name;
	scene_node->sceneGraph = this;
}

SceneNode* SceneGraph::getSceneNode(char* name) {
	return sceneNodes[name];
}

void SceneGraph::update() {
	if (isDebug) std::cout << "\nUpdating SceneGraph::name::" << name;

	camera->update();

	for (_sceneNodesIterator = sceneNodes.begin(); _sceneNodesIterator != sceneNodes.end(); ++_sceneNodesIterator) {
		_sceneNodesIterator->second->update();
	}
}

void SceneGraph::draw() {
	if (isDebug) std::cout << "\nDrawaing SceneGraph::name::" << name;

	for (_sceneNodesIterator = sceneNodes.begin(); _sceneNodesIterator != sceneNodes.end(); ++_sceneNodesIterator) {
		_sceneNodesIterator->second->draw();
	}
}

void SceneGraph::calculateRay(int mouseX, int mouseY, int winX, int winY)
{
	float x = (2.0f * (float)mouseX) / (float)winX - 1.0f;
	float y = 1.0f - (2.0f * (float)mouseY) / (float)winY;
	float z = 1.0f;

	Vector3 ray_nds = Vector3(x, y, z);
	Vector4 ray_clip = Vector4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	Vector4 ray_eye = camera->ProjectionMatrix.invert() * ray_clip;

	ray_eye = Vector4(ray_eye.x, ray_eye.y, 1.0, 0.0);

	Matrix4 viewMatrixInv = camera->ViewMatrix.invert();

	Vector4 aux = viewMatrixInv * ray_eye;
	Vector3 ray_wor = Vector3(aux.x, aux.y, aux.z);

	rayDirection = ray_wor.normalize();

	Vector4 rayOriginAux = viewMatrixInv * Vector4(0.0f, 0.0f, -camera->Distance, 0.0f);
	rayOrigin = Vector3(rayOriginAux.x, rayOriginAux.y, rayOriginAux.z);

	Vector3 rayO = rayOrigin;
	rayPoint = rayO.normalize() + rayDirection;

	checkIntersection = true;
}