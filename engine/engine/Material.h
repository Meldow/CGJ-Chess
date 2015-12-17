#pragma once

#include <iostream>

class Material {

private:

	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess;

public:

	Material(std::string& filename);

	void parseAmbient(std::stringstream& sin);
	void parseDiffuse(std::stringstream& sin);
	void parseSpecular(std::stringstream& sin);
	void parseShininess(std::stringstream& sin);
	void parseTransparency(std::stringstream& sin);

	void setAlpha(float alpha);

	void parseLine(std::stringstream& sin);
	void loadMaterialData(std::string& filename);
	const void createMaterial(std::string& filename);

	float* getAmbient();
	float* getDiffuse();
	float* getSpecular();
	float getShininess();

};