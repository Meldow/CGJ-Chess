#pragma once
#include "SceneNode.h"
#include "VSShaderLib.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "SceneGraph.h"
#include "ManagerLight.h"
#include "Light.h"

SceneNode::SceneNode() {
	modelMatrix = Matrix4().identity();
	transform = Transform();
	boundingBox = new BoundingBox();
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
	if (shaderProgram) {
		if (shaderProgram->needBlend) material->setAlpha(0.5);
	}
	shaderProgram->setUniform("mat.ambient", material->getAmbient());
	shaderProgram->setUniform("mat.diffuse", material->getDiffuse());
	shaderProgram->setUniform("mat.specular", material->getSpecular());
	shaderProgram->setUniform("mat.shininess", material->getShininess());

	Matrix4* nm = new Matrix4(modelMatrix);
	shaderProgram->setUniform("NormalMatrix", nm->invert().transpose().data());

	shaderProgram->setUniform("mlwNumPointLights", ManagerLight::instance()->getLightsCount());

}

void SceneNode::loadTextureUniforms() {
	texture->draw();
	shaderProgram->setUniform("tex_map", 0);
}


void SceneNode::updateModelMatrix() {
	modelMatrix = Matrix4().translate(transform.position)
	//TODO
	//.rotate(transform.rotation.x, Vector3(1, 0, 0))
	//.rotate(transform.rotation.y, Vector3(0, 1, 0))
	//.rotate(transform.rotation.z, Vector3(0, 0, 1))
	.scale(transform.scale.x, transform.scale.y, transform.scale.z);
}

void SceneNode::setLightUniforms() {

	//std::map<char*, PointLight*> pointLights = ManagerLight::instance()->pointLights;
	//std::map<char*, PointLight*>::iterator _pointLightIterator;

	//for (_pointLightIterator = pointLights.begin(); _pointLightIterator != pointLights.end(); ++_pointLightIterator) {
	//	PointLight* pl = _pointLightIterator->second;


	//}

	PointLight* pl = ManagerLight::instance()->getPointLight("main");
	
	shaderProgram->setUniform("pointLights.Position", pl->Position.get());
	shaderProgram->setUniform("pointLights.AmbientIntensity", pl->AmbientIntensity);
	shaderProgram->setUniform("pointLights.DiffuseIntensity", pl->DiffuseIntensity);
	shaderProgram->setUniform("pointLights.Color", pl->Color.get());
	shaderProgram->setUniform("pointLights.Atten", pl->Attenuation.get());
	shaderProgram->setUniform("pointLights.Range", pl->Range);
}

Matrix4* SceneNode::calculateGraphModelMatrix() {
	return new Matrix4();
}

void SceneNode::setUniforms() {
	//calculate model matrix
	updateModelMatrix();

	shaderProgram->setUniform("ModelMatrix", modelMatrix.data());

	if (material) loadMaterialUniforms();
	if (texture) loadTextureUniforms();
}

void SceneNode::update() {
	if (isDebug) std::cout << "\nUpdating SceneNode::name::" << name;

	boundingBox->setPosition(transform.position.x, transform.position.y, transform.position.z);

	for (_sceneNodesIterator = sceneNodes.begin(); _sceneNodesIterator != sceneNodes.end(); ++_sceneNodesIterator) {
		_sceneNodesIterator->second->update();
		if (sceneGraph->checkIntersection) {
			if (childPicked) {
				if (_sceneNodesIterator->second->objectPicked)
					_sceneNodesIterator->second->checkIntersection();
			} else {
				_sceneNodesIterator->second->checkIntersection();
			}
		} else {
			childPicked = false;
			_sceneNodesIterator->second->objectPicked = false;
		}
	}
}

void SceneNode::checkIntersection() {
	if (boundingBox->checkRayIntersection(sceneGraph->rayOrigin, sceneGraph->rayDirection)) {
		objectPicked = true;
		parentNode->childPicked = true;
	}

	if (objectPicked) {
		float camDist = sceneGraph->camera->Distance;
		transform.setPosition(sceneGraph->rayPoint.x * camDist, sceneGraph->rayPoint.y * camDist, sceneGraph->rayPoint.z * camDist);
		modelMatrix = Matrix4().translate(transform.position.x, transform.position.y, transform.position.z);
	}
}

void SceneNode::draw() {
	if (isDebug) std::cout << "\nDrawaing SceneNode::name::" << name;

	if (shaderProgram) {
		if (shaderProgram->needBlend) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glEnable(GL_CULL_FACE);
			glDepthMask(GL_FALSE);
		}
		if (shaderProgram->enableStencil) {
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF); // Write to stencil buffer
			glEnable(GL_CULL_FACE);
			glDepthMask(GL_FALSE); // Don't write to depth buffer
			glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
		}
		if (shaderProgram->disableStencil) {
			glDisable(GL_STENCIL_TEST);
			glCullFace(GL_BACK);
		}

		setUniforms();
		if (shaderProgram->affectedByLights) setLightUniforms();
		if (mesh) mesh->draw();

		if (shaderProgram->enableStencil) {
			glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
			glStencilMask(0x00); // Don't write anything to stencil buffer
			glDepthMask(GL_TRUE); // Write to depth buffer
			glCullFace(GL_FRONT);
		}

		if (shaderProgram->needBlend) {
			glDisable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			glDepthMask(GL_TRUE);
		}
	}

	for (_sceneNodesIterator = sceneNodes.begin(); _sceneNodesIterator != sceneNodes.end(); ++_sceneNodesIterator) {
		_sceneNodesIterator->second->draw();
	}
}

