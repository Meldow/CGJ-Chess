#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Dependencies/glew/glew.h"
#include "Dependencies/freeglut/freeglut.h"

typedef struct {
	GLfloat kR, kG, kB, kA;
} Ambient;

typedef struct {
	GLfloat kR, kG, kB, kA;
} Diffuse;

typedef struct {
	GLfloat kR, kG, kB, kA;
} Specular;

typedef struct {
	Ambient ambient;
	Diffuse diffuse;
	Specular specular;
	GLfloat shininess;
} Mat;

class Material {

private:

	Mat _mat;

public:

	Material(std::string& filename);

	void parseAmbient(std::stringstream& sin);
	void parseDiffuse(std::stringstream& sin);
	void parseSpecular(std::stringstream& sin);
	void parseShininess(std::stringstream& sin);
	void parseTransparency(std::stringstream& sin);

	void parseLine(std::stringstream& sin);
	void loadMaterialData(std::string& filename);
	const void createMaterial(std::string& filename);

	float* getAmbient();
	float* getDiffuse();
	float* getSpecular();
	float* getShininess();

};