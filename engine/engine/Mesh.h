#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Dependencies/glew/glew.h"
#include "Dependencies/freeglut/freeglut.h"
#include "Matrices.h"

typedef struct {
	GLfloat x, y, z;
} Vertex;

typedef struct {
	GLfloat u, v;
} Texcoord;

typedef struct {
	GLfloat nx, ny, nz;
} Normal;

class Mesh {

private:

	bool texCoordsLoaded, normalsLoaded;

	std::vector <Vertex> vertices, vertexData;
	std::vector <Texcoord> texCoords, texcoordData;
	std::vector <Normal> normals, normalData;

	std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

public:

	static const int VERTICES = 0;
	static const int TEXCOORDS = 1;
	static const int NORMALS = 2;

	Mesh(std::string& filename);

	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseFace(std::stringstream& sin);
	void parseLine(std::stringstream& sin);
	void loadMeshData(std::string& filename);
	void processMeshData();
	void freeMeshData();
	const void createMesh(std::string& filename);

	//VAOs and VBOs
	void createBufferObjects();
	void destroyBufferObjects();

	void bindVertexArray();

	void draw();

};