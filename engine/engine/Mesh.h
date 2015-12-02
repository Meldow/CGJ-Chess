#pragma once
#include "Dependencies/glew/glew.h"

const GLuint UBO_BP = 0;	//keep this one

class Mesh
{
private:
	GLuint VaoId, VboVertices, VboCamera = -1;
public:
	static const int VERTICES = 0;
	static const int COLORS = 1;

	Mesh();
	~Mesh();
	void createBufferObjects();
	void draw();
};