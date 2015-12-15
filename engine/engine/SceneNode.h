#pragma once
#include <map>
#include "Matrices.h"

class VSShaderLib;
class Mesh;
class Material;
class Texture;
class SceneGraph;
class SceneNode {
public:
	SceneNode* parentNode;
	SceneGraph* sceneGraph;

	VSShaderLib* shaderProgram;
	Matrix4 modelMatrix;
	Mesh* mesh;
	Material* material;
	Texture* texture;

	char* name;
	//BoxCollider* collider;
	bool isDebug = false;

	SceneNode();
	~SceneNode();

	void addSceneNode(char* name, SceneNode* scene_node);
	SceneNode* getSceneNode(char* name);
	//update
	void update();
	void draw();
private:
	std::map<const char*, SceneNode*> sceneNodes;
	std::map<const char*, SceneNode*>::iterator _sceneNodesIterator;

	void setUniforms();
	void loadMaterialUniforms();
	void loadTextureUniforms();
};