#pragma once
#include "BaseLight.h"

BaseLight::BaseLight() {
	this->transform = Transform();
}

BaseLight::BaseLight(Transform transform) : transform(transform) {}

BaseLight::~BaseLight() {}