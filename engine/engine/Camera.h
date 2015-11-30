#pragma once

#include "Matrices.h"
#include "Dependencies/glew/glew.h"

class Camera {
public:
	Matrix4 ViewMatrix;
	Matrix4 OrthoProjectionMatrix;
	Matrix4 PerspectiveProjectionMatrix;
	float Distance = 10;
	bool isOrtho = false;
	GLint VboID;

	Camera();
	~Camera();

	Matrix4 LookAt(Vector3& eye, Vector3& center, Vector3& up);
	Vector3 getSide(Vector3& eye, Vector3& center, Vector3& up);
	Vector3 getForward(Vector3& eye, Vector3& center);
	Matrix4 Ortho(float left, float right, float bottom, float top, float mNear, float mFar);
	Matrix4 Perspective(float fov, float aspect, float mNear, float mFar);
	Matrix4 Perspective(float fov, float width, float height, float mNear, float mFar);

	void draw();
};