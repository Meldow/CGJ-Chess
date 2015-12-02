#pragma once
#include "Singleton.h"
#include "Dependencies/glew/glew.h"
#include <map>

class SceneGraph;

class ManagerSceneGraph {
	SINGLETON_HEADER(ManagerSceneGraph)
public:
	char* name;
	std::map<const char*, SceneGraph*> sceneGraphs;		//contains ids of all uniforms

	void addSceneGraph(const char* name, SceneGraph* scene_graph);
	SceneGraph* getSceneGraph(char* name);

private:
	std::map<const char*, GLint>::iterator _sceneGraphsIterator;

};


