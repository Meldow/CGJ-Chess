#pragma once
#include "Singleton.h"
#include <map>

struct DirectionalLight;
struct PointLight;
struct SpotLight;

class ManagerLight {
	SINGLETON_HEADER(ManagerLight)
public:
	char* name;
	std::map<char*, DirectionalLight*> directionalLights;
	std::map<char*, PointLight*> pointLights;
	std::map<char*, SpotLight*> spotLights;

	void addDirectionalLight(char* name, DirectionalLight* directionalLight);
	void addPointLight(char* name, PointLight* pointLight);
	void addSpotLight(char* name, SpotLight* spotLight);
	
	DirectionalLight* getDirectionalLight(char* name);
	PointLight* getPointLight(char* name);
	SpotLight* getSpotLight(char* name);

private:
	const unsigned int MAX_POINT_LIGHTS = 4;
	const unsigned int MAX_SPOT_LIGHTS = 4;
	std::map<char*, DirectionalLight*>::iterator _directionalLightIterator;
	std::map<char*, PointLight*>::iterator _pointLightIterator;
	std::map<char*, SpotLight*>::iterator _spotLightIterator;
};

