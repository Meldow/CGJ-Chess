#pragma once
#include "SceneNode.h"
#include "VSShaderLib.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "SceneGraph.h"

SceneNode::SceneNode() {
	modelMatrix = Matrix4().identity();
	transform = Transform();
	boundingBox = new BoundingBox(0.607f, 1.091f, 0.607f);
}

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

void SceneNode::loadMaterialUniforms() {
	shaderProgram->setUniform("mat.ambient", material->getAmbient());
	shaderProgram->setUniform("mat.diffuse", material->getDiffuse());
	shaderProgram->setUniform("mat.specular", material->getSpecular());
	shaderProgram->setUniform("mat.shininess", material->getShininess());

}

void SceneNode::loadTextureUniforms() {
	texture->draw();
	shaderProgram->setUniform("tex_map", 0);
}

void SceneNode::setUniforms() {
	shaderProgram->setUniform("ModelMatrix", modelMatrix.data());

	if(material) loadMaterialUniforms();
	if(texture) loadTextureUniforms();
}

void SceneNode::update() {
	if (isDebug) std::cout << "\nUpdating SceneNode::name::" << name;

	if (sceneGraph->checkIntersection) {
		if (boundingBox->checkRayIntersection(sceneGraph->rayOrigin, sceneGraph->rayDirection)) {
			objectPicked = true;
		}
	}
	else objectPicked = false;

	if (objectPicked) {
		float camDist = sceneGraph->camera->Distance;
		transform.setPosition(sceneGraph->rayPoint.x * camDist, sceneGraph->rayPoint.y * camDist, sceneGraph->rayPoint.z * camDist);
		modelMatrix = Matrix4().translate(transform.position.x, transform.position.y, transform.position.z);
	}

	boundingBox->setPosition(transform.position.x, transform.position.y, transform.position.z);
}

void SceneNode::draw() {
	if (isDebug) std::cout << "\nDrawaing SceneNode::name::" << name;

	if (shaderProgram) {
		setUniforms();
		if(mesh) mesh->draw();
	}

	for (_sceneNodesIterator = sceneNodes.begin(); _sceneNodesIterator != sceneNodes.end(); ++_sceneNodesIterator) {
		_sceneNodesIterator->second->draw();
	}
}

