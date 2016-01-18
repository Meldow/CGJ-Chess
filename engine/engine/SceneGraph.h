#pragma once
#include <map>
#include "Camera.h"

class SceneNode;
class SceneGraph {
public:
	Camera* camera;
	char* name;

	Vector3 rayOrigin;
	Vector3 rayDirection;
	Vector3 rayPoint;
	bool checkIntersection = false;
	bool pickingTable = false;
	bool pickingBoard = false;
	bool pickingPiece = false;

	bool isDebug = false;

	SceneGraph();
	~SceneGraph();

	void addSceneNode(char* name, SceneNode* scene_node);
	SceneNode* getSceneNode(char* name);

	void update();
	void draw();

	void calculateRay(int mouseX, int mouseY, int winX, int winY);
private:
	std::map<const char*, SceneNode*> sceneNodes;
	std::map<const char*, SceneNode*>::iterator _sceneNodesIterator;
};