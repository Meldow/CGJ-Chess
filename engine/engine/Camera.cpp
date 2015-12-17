#include "Camera.h"

GLuint VboCamera;

Camera::Camera(GLuint UBO_BP)
{
	ViewMatrix = Matrix4().identity();
	ProjectionMatrix = Matrix4().identity();
	RotationMatrix = Matrix4().identity();
	

	createUniformBuffer(UBO_BP);
}

Camera::~Camera()
{
	//Destroy uniform buffer
}

void Camera::createUniformBuffer(GLuint UBO_BP)
{
	glGenBuffers(1, &VboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, VboCamera);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(Matrix4), 0, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, VboCamera);
}

Matrix4 Camera::perspective(float fov, float aspect, float mNear, float mFar) {

	float d = 1.0f / tan(fov * (M_PI / 360.0f));

	Matrix4 res = Matrix4();
	res.m[0] = d / aspect;
	res.m[5] = d;
	res.m[10] = (mNear + mFar) / (mNear - mFar);
	res.m[11] = -1.0f;
	res.m[14] = (2.0f * mFar * mNear) / (mNear - mFar);
	res.m[15] = 0.0f;
	return res;
}

void Camera::update()
{
	ViewMatrix = Matrix4().translate(0.0f, 0.0f, -Distance) * RotationMatrix;
	ProjectionMatrix = perspective(30, 800.0f / 600.0f, 1, 200);
	
	glBindBuffer(GL_UNIFORM_BUFFER, VboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ViewMatrix.m), ViewMatrix.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, VboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(ViewMatrix.m), sizeof(ProjectionMatrix.m), ProjectionMatrix.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

