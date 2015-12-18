#pragma once
#include "Vectors.h"

class Matrix4;

struct BaseLight {
	Vector3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight() : Color(0.0f, 0.0f, 0.0f), AmbientIntensity(0.0f), DiffuseIntensity(0.0f) {}
};

struct DirectionalLight : public BaseLight {
	Vector3 Direction;

	DirectionalLight() : Direction(0.0f, 0.0f, 0.0f) {}
};

struct PointLight : public BaseLight {
	Vector3 Position;
	float Range;
	Vector3 Attenuation;

	PointLight() : Position(0.0f, 0.0f, 0.0f), Range(10), Attenuation(1.0f, 0.045f, 0.0075f) {}
};

struct SpotLight : public PointLight {
	Vector3 Direction;
	float Cutoff;

	SpotLight() : Direction(0.0f, 0.0f, 0.0f), Cutoff(0.0f) {}
};
