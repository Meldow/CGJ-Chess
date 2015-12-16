#pragma once
#include "Vectors.h"

class Transform {
public:
	Transform();
	~Transform();

private:
	Vector3 position;
	Vector3 scale;
	Vector4 rotation;

	// setters
	void setPosition(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setRotation(float angle, float x, float y, float z);

	//getters
	Vector3 getPosition();
	Vector3 getScale();
	Vector4 getRotation();

};