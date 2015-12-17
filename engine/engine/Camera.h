#pragma once

#include "Dependencies/glew/glew.h"
#include "Matrices.h"

class Camera {
public:

	Matrix4 ViewMatrix;
	Matrix4 ProjectionMatrix;
	Matrix4 RotationMatrix;

	float Distance = 15.0f;

	Camera(GLuint UBO_BP);
	~Camera();

	void createUniformBuffer(GLuint UBO_BP);

	Matrix4 perspective(float fov, float aspect, float mNear, float mFar);

	void update();

};
