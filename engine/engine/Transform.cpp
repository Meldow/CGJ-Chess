#pragma once
#include "Transform.h"
#include <string>

Transform::Transform() {
	Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
	Vector4 rotation = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
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

void Transform::setRotation(float angle, float x, float y, float z) {
	rotation.w = angle;
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}