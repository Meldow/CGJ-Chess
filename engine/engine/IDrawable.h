#pragma once

class IDrawable {
public:
	virtual void draw() = 0;
	virtual ~IDrawable() {}
};