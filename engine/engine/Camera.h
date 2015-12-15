#pragma once

#include "Dependencies/glew/glew.h"
#include "Matrices.h"

class Camera {

private:

	Matrix4 ViewMatrix;
	Matrix4 ProjectionMatrix;
	Matrix4 RotationMatrix;

public:

	float Distance = 15.0f;

	Camera(GLuint UBO_BP);
	~Camera();

	void createUniformBuffer(GLuint UBO_BP);

	void setRotationMatrix(Matrix4 rotation);

	Matrix4 perspective(float fov, float aspect, float mNear, float mFar);

	void update();

};
