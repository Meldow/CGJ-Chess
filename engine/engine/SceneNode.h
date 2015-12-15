#pragma once
#include <map>
#include "Matrices.h"

class ShaderProgram;
class Mesh;
class SceneGraph;
class SceneNode {
public:
	SceneNode* parentNode;
	SceneGraph* sceneGraph;
	//Transform transform;
	ShaderProgram* shaderProgram;
	Matrix4 modelMatrix;
	Mesh* mesh;
	char* name;
	//BoxCollider* collider;
	bool isDebug = false;

	SceneNode();
	~SceneNode();

	void addSceneNode(char* name, SceneNode* scene_node);
	SceneNode* getSceneNode(char* name);
	//update
	void draw();
private:
	std::map<const char*, SceneNode*> sceneNodes;
	std::map<const char*, SceneNode*>::iterator _sceneNodesIterator;
};