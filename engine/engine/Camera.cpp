#include "Camera.h"
#include "ManagerOpenGLErrors.h"

Camera::Camera() {

}

Camera::~Camera() {}

Matrix4 Camera::LookAt(Vector3& eye, Vector3& center, Vector3& up) {
	Vector3 forward, side, up2;
	Matrix4 R, T;
	////------------------ 1
	forward = center - eye;
	forward = forward.normalize();

	side = forward.cross(up);
	side = side.normalize();
	////------------------
	up2 = side.cross(forward);
	////------------------ 2 Rotation Matrix
	R = Matrix4(side[0], side[1], side[2], 0.0,
		up2[0], up2[1], up2[2], 0.0,
		-forward[0], -forward[1], -forward[2], 0.0,
		0.0f, 0.0f, 0.0f, 1.0f).transpose();
	////------------------ 3 Apply translation
	T = Matrix4(1.0f, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		-eye[0], -eye[1], -eye[2], 1);
	////------------------ 4 M = TR
	Matrix4 result = (R * T);
	//std::cout << "\nResult\n" << result << "\n\n\n";
	return result;
}

Vector3 Camera::getSide(Vector3& eye, Vector3& center, Vector3& up) {
	Vector3 forward, side;
	forward = center - eye;
	forward = forward.normalize();

	side = forward.cross(up);
	return side = side.normalize();
}

Vector3 Camera::getForward(Vector3& eye, Vector3& center) {
	Vector3 forward;
	forward = center - eye;
	return forward.normalize();
}

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
	if (mNear <= 0.0f || mFar <= 0.0f) { std::cout << "ERROR: Near/Far == 0."; return{}; }

	float delta = fov / 2;
	float d = 1 / tan(delta);

	Matrix4 result = Matrix4(d / aspect, 0.0f, 0.0f, 0.0f,
		0.0f, d, 0.0f, 0.0f,
		0.0f, 0.0f, (mNear + mFar) / (mNear - mFar), -1.0f,
		0.0f, 0.0f, (2 * mFar*mNear) / (mNear - mFar), 0.0f);
	//std::cout << "\nResult\n" << result << "\n\n\n";
	return result;
}

Matrix4 Camera::Perspective(float fov, float width, float height, float mNear, float mFar) {
	return Perspective(fov, width / height, mNear, mFar);
}

void Camera::draw() {
	glBindBuffer(GL_UNIFORM_BUFFER, VboID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ViewMatrix.m), ViewMatrix.m);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(ViewMatrix.m), sizeof(PerspectiveProjectionMatrix.m), PerspectiveProjectionMatrix.m);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not draw camera.");
}
