#pragma once
#include "BaseLight.h"

struct PointLight : public BaseLight {
	struct {
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;
};