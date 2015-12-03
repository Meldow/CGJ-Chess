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

void SceneGraph::draw() {
	if (isDebug) std::cout << "\nDrawaing SceneGraph::name::" << name;
	
	for (_sceneNodesIterator = sceneNodes.begin(); _sceneNodesIterator != sceneNodes.end(); ++_sceneNodesIterator) {
		_sceneNodesIterator->second->draw();
	}
}