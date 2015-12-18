#pragma once
#include "Dependencies/glew/glew.h"
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
	struct {
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight() : Position(0.0f, 0.0f, 0.0f), Range(10) {
		Attenuation.Constant = 1.0f;
		Attenuation.Linear = 0.045f;
		Attenuation.Exp = 0.0075f;
	}
};

struct SpotLight : public PointLight {
	Vector3 Direction;
	float Cutoff;

	SpotLight() : Direction(0.0f, 0.0f, 0.0f), Cutoff(0.0f) {}
};

struct mlwDirectionalLight {
	GLuint Color;
	GLuint AmbientIntensity;
	GLuint DiffuseIntensity;
	GLuint Direction;
};

struct mlwPointLight {
	GLuint Color;
	GLuint AmbientIntensity;
	GLuint DiffuseIntensity;
	GLuint Position;
	struct {
		GLuint Constant;
		GLuint Linear;
		GLuint Exp;
	} Atten;
};

struct mlwSpotLight {
	GLuint Color;
	GLuint AmbientIntensity;
	GLuint DiffuseIntensity;
	GLuint Position;
	GLuint Direction;
	GLuint Cutoff;
	struct {
		GLuint Constant;
		GLuint Linear;
		GLuint Exp;
	} Atten;
};

//class Lighting {
//public:
//
//
//
//	Lighting();
//
//	void SetDirectionalLight(const DirectionalLight& Light);
//	void SetPointLights(unsigned int NumLights, const PointLight* pLights);
//	void SetSpotLights(unsigned int NumLights, const SpotLight* pLights);
//
//private:
//	GLuint m_numPointLightsLocation;
//	GLuint m_numSpotLightsLocation;
//
//	
//};