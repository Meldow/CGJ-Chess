#include "Quaternion.h"

#define DELTA 1e-6
#include <string>

Quaternion::Quaternion() {
	_t = 1.0f;
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
}

Quaternion::Quaternion(float angle, Vector3& v) {
	float a = angle * (float)DEGREES_TO_RADIANS;
	_t = cos(a / 2.0f);
	float si = sin(a / 2.0f);
	_x = v.x * si;
	_y = v.y * si;
	_z = v.z * si;

	float quadrance = _t * _t + _x * _x + _y * _y + _z * _z;
	float norm = sqrt(quadrance);
	float s = 1 / norm;

	_t = _t * s;
	_x = _x * s;
	_y = _y * s;
	_z = _z * s;
}

Quaternion::~Quaternion() {}

void Quaternion::set(float angle, Vector3& v) {
	float a = angle * (float)DEGREES_TO_RADIANS;
	_t = cos(a / 2.0f);
	float si = sin(a / 2.0f);
	_x = v.x * si;
	_y = v.y * si;
	_z = v.z * si;

	float quadrance = _t * _t + _x * _x + _y * _y + _z * _z;
	float norm = sqrt(quadrance);
	float s = 1 / norm;

	_t = _t * s;
	_x = _x * s;
	_y = _y * s;
	_z = _z * s;
}

float Quaternion::getX() {
	return _x;
}

float Quaternion::getY() {
	return _y;
}

float Quaternion::getZ() {
	return _z;
}

float Quaternion::getT() {
	return _t;
}

Quaternion& Quaternion::operator=(const Quaternion& q) {
	_x = q._x;
	_y = q._y;
	_z = q._z;
	_t = q._t;
	return *this;
}

Quaternion operator*(const Quaternion& q0, const Quaternion& q1) {
	Quaternion q;
	q._t = q0._t * q1._t - q0._x * q1._x - q0._y * q1._y - q0._z * q1._z;
	q._x = q0._t * q1._x + q0._x * q1._t + q0._y * q1._z - q0._z * q1._y;
	q._y = q0._t * q1._y + q0._y * q1._t + q0._z * q1._x - q0._x * q1._z;
	q._z = q0._t * q1._z + q0._z * q1._t + q0._x * q1._y - q0._y * q1._x;
	return q;
}

Quaternion operator*(const Quaternion& q, float s) {
	float t, x, y, z;
	t = s * q._t;
	x = s * q._x;
	y = s * q._y;
	z = s * q._z;
	return Quaternion(t, Vector3(x, y, z));
}

Quaternion Quaternion::normalize() {
	float quadrance = _t * _t + _x * _x + _y * _y + _z * _z;
	float norm = sqrt(quadrance);
	float s = 1 / norm;
	float t, x, y, z;
	t = s * _t;
	x = s * _x;
	y = s * _y;
	z = s * _z;
	return Quaternion(t, Vector3(x, y, z));
}

Quaternion Quaternion::inverse(const Quaternion& q) {
	float t, x, y, z;
	x = q._x;
	y = q._y;
	z = q._z;
	t = q._t;
	return Quaternion(t, -Vector3(x, y, z));
}

Quaternion Quaternion::eulerToQuaternion(float roll, float pitch, float yaw) {
	float cr, cp, cy, sr, sp, sy, cpcy, spsy;
	float t, x, y, z;
	// calculate trig identities
	cr = cos(roll / 2);
	cp = cos(pitch / 2);
	cy = cos(yaw / 2);
	sr = sin(roll / 2);
	sp = sin(pitch / 2);
	sy = sin(yaw / 2);
	cpcy = cp * cy;
	spsy = sp * sy;
	t = cr * cpcy + sr * spsy;
	x = sr * cpcy - cr * spsy;
	y = cr * sp * cy + sr * cp * sy;
	z = cr * cp * sy - sr * sp * cy;
	return Quaternion(t, Vector3(x, y, z));
}

Quaternion Quaternion::matrixToQuaternion(Matrix4 matrix) {
	float t, x, y, z;
	float *m = matrix.data();
	Quaternion q;

	float trace = m[0] + m[5] + m[10];

	if (trace > 0) {// I changed M_EPSILON to 0
		float s = 0.5f / sqrtf(trace + 1.0f);
		q._t = 0.25f / s;
		q._x = (m[6] - m[9]) * s;
		q._y = (m[8] - m[2]) * s;
		q._z = (m[1] - m[4]) * s;
	} else {
		if (m[0] > m[5] && m[0] > m[10]) {
			float s = 2.0f * sqrtf(1.0f + m[0] - m[5] - m[10]);
			q._t = (m[6] - m[9]) / s;
			q._x = 0.25f * s;
			q._y = (m[4] + m[1]) / s;
			q._z = (m[8] + m[2]) / s;
		} else if (m[5] > m[10]) {
			float s = 2.0f * sqrtf(1.0f + m[5] - m[0] - m[10]);
			q._t = (m[8] - m[2]) / s;
			q._x = (m[4] + m[1]) / s;
			q._y = 0.25f * s;
			q._z = (m[9] + m[6]) / s;
		} else {
			float s = 2.0f * sqrtf(1.0f + m[10] - m[0] - m[5]);
			q._t = (m[1] - m[4]) / s;
			q._x = (m[8] + m[2]) / s;
			q._y = (m[9] + m[6]) / s;
			q._z = 0.25f * s;
		}
	}
	return q;
}

Matrix4 Quaternion::quaternionToMatrix() {

	float matrix[16];
	// calculate coefficients
	float xx = _x * _x;
	float xy = _x * _y;
	float xz = _x * _z;
	float xt = _x * _t;
	float yy = _y * _y;
	float yz = _y * _z;
	float yt = _y * _t;
	float zz = _z * _z;
	float zt = _z * _t;

	matrix[0] = 1.0f - 2.0f * (yy + zz);
	matrix[1] = 2.0f * (xy + zt);
	matrix[2] = 2.0f * (xz - yt);
	matrix[3] = 0.0f;

	matrix[4] = 2.0f * (xy - zt);
	matrix[5] = 1.0f - 2.0f * (xx + zz);
	matrix[6] = 2.0f * (yz + xt);
	matrix[7] = 0.0f;

	matrix[8] = 2.0f * (xz + yt);
	matrix[9] = 2.0f * (yz - xt);
	matrix[10] = 1.0f - 2.0f * (xx + yy);
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;

	return Matrix4(matrix);
}

Quaternion Quaternion::slerp(const Quaternion& from, const Quaternion& to, float t) {
	float to1[4];
	double omega, cosom, sinom, scale0, scale1;
	float x, y, z, w;
	// calc cosine
	cosom = from._x * to._x + from._y * to._y + from._z * to._z + from._t * to._t;
	// adjust signs (if necessary)
	if (cosom < 0.0) {
		cosom = -cosom; to1[0] = -to._x;
		to1[1] = -to._y;
		to1[2] = -to._z;
		to1[3] = -to._t;
	} else {
		to1[0] = to._x;
		to1[1] = to._y;
		to1[2] = to._z;
		to1[3] = to._t;
	}
	// calculate coefficients
	if ((1.0 - cosom) > DELTA) {
		// standard case (slerp)
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0 - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	} else {
		// "from" and "to" quaternions are very close 
		//  so we can do a linear interpolation
		scale0 = 1.0 - t;
		scale1 = t;
	}
	x = scale0 * from._x + scale1 * to1[0];
	y = scale0 * from._y + scale1 * to1[1];
	z = scale0 * from._z + scale1 * to1[2];
	w = scale0 * from._t + scale1 * to1[3];

	return Quaternion(t, Vector3(x, y, z));
}

std::string Quaternion::toString() {
	return "[ " + std::to_string(_t) + " ; " + std::to_string(_x) + " ; " + std::to_string(_y) + " ; " + std::to_string(_z) + " ]";
}
