#pragma once
#include "ManagerSceneGraph.h"

SINGLETON_IMPLEMENTATION_NO_CONSTRUCTOR(ManagerSceneGraph)

ManagerSceneGraph::ManagerSceneGraph() {}

void ManagerSceneGraph::addSceneGraph(const char* name, SceneGraph* scene_graph) {
	sceneGraphs.insert(std::pair<const char*, SceneGraph*>(name, scene_graph));
}

SceneGraph* ManagerSceneGraph::getSceneGraph(char* name) {
	return sceneGraphs[name];
}
