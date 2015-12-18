#pragma once
#include "ManagerLight.h"
#include "Light.h"
#include <iostream>

SINGLETON_IMPLEMENTATION_NO_CONSTRUCTOR(ManagerLight)

ManagerLight::ManagerLight() {}

std::map<char*, PointLight*> ManagerLight::getPointLights() {
	return pointLights;
}

void ManagerLight::addDirectionalLight(char* name, DirectionalLight* directionalLight) {
	directionalLights.insert(std::pair<char*, DirectionalLight*>(name, directionalLight));
}

void ManagerLight::addPointLight(char* name, PointLight* pointLight) {
	if (pointLights.size() < MAX_POINT_LIGHTS)
		pointLights.insert(std::pair<char*, PointLight*>(name, pointLight));
	else
		std::cout << "\nManagerLight::addPointLight::Maximum number POINT lights reached when adding " << name << ".";
}

void ManagerLight::addSpotLight(char* name, SpotLight* spotLight) {
	if (spotLights.size() < MAX_POINT_LIGHTS)
		spotLights.insert(std::pair<char*, SpotLight*>(name, spotLight));
	else
		std::cout << "\nManagerLight::addPointLight::Maximum number SPOT lights reached when adding " << name << ".";
}

DirectionalLight* ManagerLight::getDirectionalLight(char* name) {
	try {
		return directionalLights.find(name)->second;
	} catch (std::exception) {
		std::cout << "\nManagerLight::getDirectionalLight::Element not found::" << name;
	}
	std::cout << "\nManagerLight::getDirectionalLight::Element not found::" << name;
	return nullptr;
}

PointLight* ManagerLight::getPointLight(char* name) {
	try {
		return pointLights.find(name)->second;
	} catch (std::exception) {
		std::cout << "\nManagerLight::getPointLight::Element not found::" << name;
	}
	std::cout << "\nManagerLight::getPointLight::Element not found::" << name;
	return nullptr;
}

SpotLight* ManagerLight::getSpotLight(char* name) {
	try {
		return spotLights.find(name)->second;
	} catch (std::exception) {
		std::cout << "\nManagerLight::getSpotLight::Element not found::" << name;
	}
	std::cout << "\nManagerLight::getSpotLight::Element not found::" << name;
	return nullptr;
}

int ManagerLight::getLightsCount() {
	return directionalLights.size() + pointLights.size() + spotLights.size();
}