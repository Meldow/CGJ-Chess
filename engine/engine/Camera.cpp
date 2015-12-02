#include "Camera.h"
#include "ManagerOpenGLErrors.h"

Camera::Camera() {
	ViewMatrix.translate(0.0f, 0.0f, -Distance);
	OrthoProjectionMatrix = Ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 50.0f);
	PerspectiveProjectionMatrix = Perspective(30, 640.0f / 480.0f, 1, 50);
}

Camera::~Camera() {}

Matrix4 Camera::Ortho(float left, float right, float bottom, float top, float mNear, float mFar) {
	if (mNear <= 0.0f || mFar <= 0.0f) { std::cout << "ERROR: Near/Far == 0."; return{}; }

	Matrix4 result = Matrix4(2 / (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 2 / (top - bottom), 0.0f, 0.0f,
		0.0f, 0.0f, -2 / (mFar - mNear), 0.0f,
		(left + right) / (left - right), (bottom + top) / (bottom - top), (mNear + mFar) / (mNear - mFar), 1.0f);
	//std::cout << "\nResult\n" << result << "\n\n\n";
	return result;
}

Matrix4 Camera::Perspective(float fov, float aspect, float mNear, float mFar) {

	float d = 1.0f / tan(fov * (M_PI / 360.0f));

	Matrix4 res = Matrix4();
	res.m[0] = d / aspect;
	res.m[5] = d;
	res.m[10] = (mNear + mFar) / (mNear - mFar);
	res.m[11] = -1.0f;
	res.m[14] = (2.0f * mFar * mNear) / (mNear - mFar);
	res.m[15] = 0.0f;
	std::cout << "\nres::\n" << res;
	return res;
}

Matrix4 Camera::Perspective(float fov, float width, float height, float mNear, float mFar) {
	return Perspective(fov, width / height, mNear, mFar);
}

void Camera::draw() {
	if (isDebug) {
		std::cout << "\nViewMatrix\n" << ViewMatrix;
		if (isOrtho) std::cout << "\nOrthoProjectionMatrix\n" << OrthoProjectionMatrix;
		else std::cout << "\nPerspectiveProjectionMatrix\n" << PerspectiveProjectionMatrix;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, VboID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ViewMatrix.m), ViewMatrix.m);
	if(isOrtho)
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(ViewMatrix.m), sizeof(OrthoProjectionMatrix.m), OrthoProjectionMatrix.m);
	else
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(ViewMatrix.m), sizeof(PerspectiveProjectionMatrix.m), PerspectiveProjectionMatrix.m);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not draw camera.");
}
