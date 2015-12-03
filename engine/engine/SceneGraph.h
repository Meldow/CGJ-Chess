#pragma once
#include <map>
#include "Camera.h"

class SceneNode;
class SceneGraph {
public:
	Camera* camera;
	char* name;

	bool isDebug = false;

	SceneGraph();
	~SceneGraph();

	void addSceneNode(char* name, SceneNode* scene_node);
	SceneNode* getSceneNode(char* name);

	void draw();

private:
	std::map<const char*, SceneNode*> sceneNodes;
	std::map<const char*, SceneNode*>::iterator _sceneNodesIterator;
};