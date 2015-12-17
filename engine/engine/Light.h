#pragma once
#include "Dependencies/glew/glew.h"
#include "Vectors.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define SNPRINTF _snprintf_s

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

	struct {
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight() : Position(0.0f, 0.0f, 0.0f) {
		Attenuation.Constant = 1.0f;
		Attenuation.Linear = 0.0f;
		Attenuation.Exp = 0.0f;
	}
};

struct SpotLight : public PointLight {
	Vector3 Direction;
	float Cutoff;

	SpotLight() : Direction(0.0f, 0.0f, 0.0f), Cutoff(0.0f) {}
};

class Lighting {
public:

	static const unsigned int MAX_POINT_LIGHTS = 4;
	static const unsigned int MAX_SPOT_LIGHTS = 4;

	Lighting();

	void SetDirectionalLight(const DirectionalLight& Light);
	void SetPointLights(unsigned int NumLights, const PointLight* pLights);
	void SetSpotLights(unsigned int NumLights, const SpotLight* pLights);

private:
	GLuint m_numPointLightsLocation;
	GLuint m_numSpotLightsLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Direction;
	} m_dirLightLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_pointLightsLocation[MAX_POINT_LIGHTS];

	struct {
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
	} m_spotLightsLocation[MAX_SPOT_LIGHTS];
};