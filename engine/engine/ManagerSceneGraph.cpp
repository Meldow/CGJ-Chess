#pragma once
#include "ManagerSceneGraph.h"
#include "SceneGraph.h"

SINGLETON_IMPLEMENTATION_NO_CONSTRUCTOR(ManagerSceneGraph)

ManagerSceneGraph::ManagerSceneGraph() {}

void ManagerSceneGraph::addSceneGraph(char* name, SceneGraph* scene_graph) {
	sceneGraphs.insert(std::pair<char*, SceneGraph*>(name, scene_graph));
	scene_graph->name = name;
}

SceneGraph* ManagerSceneGraph::getSceneGraph(char* name) {
	return sceneGraphs[name];
}
