#pragma once
#include "Transform.h"
#include <string>

Transform::Transform() {
	position = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	rotation = Vector3(0.0f, 0.0f, 0.0f);
}

Transform::~Transform() {}

// setters
void Transform::setPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

void Transform::setScale(float x, float y, float z) {
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

void Transform::setRotation(float x, float y, float z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}