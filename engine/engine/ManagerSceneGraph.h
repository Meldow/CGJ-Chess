#pragma once
#include "Singleton.h"
#include "Dependencies/glew/glew.h"
#include <map>

class SceneGraph;
class ManagerSceneGraph {
	SINGLETON_HEADER(ManagerSceneGraph)
public:
	char* name;
	std::map<char*, SceneGraph*> sceneGraphs;		//contains ids of all uniforms

	void addSceneGraph(char* name, SceneGraph* scene_graph);
	SceneGraph* getSceneGraph(char* name);

private:
	std::map<char*, GLint>::iterator _sceneGraphsIterator;

};


