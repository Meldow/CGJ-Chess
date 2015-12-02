#pragma once

#include "Matrices.h"
#include "Dependencies/glew/glew.h"

class Camera {
public:
	Matrix4 ViewMatrix;
	Matrix4 OrthoProjectionMatrix;
	Matrix4 PerspectiveProjectionMatrix;
	float Distance = -5.0f;
	bool isOrtho = false;
	GLint VboID;

	bool isDebug = false;

	Camera();
	~Camera();

	Matrix4 Ortho(float left, float right, float bottom, float top, float mNear, float mFar);
	Matrix4 Perspective(float fov, float aspect, float mNear, float mFar);
	Matrix4 Perspective(float fov, float width, float height, float mNear, float mFar);

	void draw();
};