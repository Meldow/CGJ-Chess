#pragma once
#include "Transform.h"

struct BaseLight {
public:
	Transform transform;
	Vector3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight();
	BaseLight(Transform transform);
	~BaseLight();
};