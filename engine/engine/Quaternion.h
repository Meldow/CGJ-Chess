#pragma once

#include <iostream>
#include "Vectors.h"
#include "Matrices.h"

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

class Quaternion {

private:
	float _t;
	float _x;
	float _y;
	float _z;

public:
	Quaternion();
	Quaternion(float angle, Vector3& v);

	~Quaternion();

	void set(float angle, Vector3& v);

	float getT();
	float getX();
	float getY();
	float getZ();


	Quaternion& operator=(const Quaternion& q);
	friend Quaternion operator*(const Quaternion& p, const Quaternion& q);
	friend Quaternion operator*(const Quaternion& p, float s);

	Quaternion normalize();

	Quaternion inverse(const Quaternion& p);

	Quaternion eulerToQuaternion(float roll, float pitch, float yaw);
	Matrix4 quaternionToMatrix();
	Quaternion matrixToQuaternion(Matrix4 m);
	Quaternion slerp(const Quaternion& p, const Quaternion& q, float t);

	std::string toString();
};