#pragma once
#include "SceneNode.h"

SceneNode::SceneNode() {}

SceneNode::~SceneNode() {}

void SceneNode::addSceneNode(char* name, SceneNode* scene_node) {
	sceneNodes.insert(std::pair<char*, SceneNode*>(name, scene_node));
	scene_node->name = name;
	scene_node->parentNode = this;
	scene_node->sceneGraph = sceneGraph;
}

SceneNode* SceneNode::getSceneNode(char* name) {
	return sceneNodes[name];
}

void SceneNode::draw() {
	if (isDebug) std::cout << "\nDrawaing SceneNode::name::" << name;
	
	for (_sceneNodesIterator = sceneNodes.begin(); _sceneNodesIterator != sceneNodes.end(); ++_sceneNodesIterator) {
		_sceneNodesIterator->second->draw();
	}
}