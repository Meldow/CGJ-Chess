#include "Material.h"
#include <string>
#include <sstream>
#include <fstream>
#include "Dependencies/glew/glew.h"

Material::Material(std::string& filename) {
	createMaterial(filename);
}

void Material::parseAmbient(std::stringstream& sin) {
	sin >> ambient[0] >> ambient[1] >> ambient[2];
}

void Material::parseDiffuse(std::stringstream& sin) {
	sin >> diffuse[0] >> diffuse[1] >> diffuse[2];
}

void Material::parseSpecular(std::stringstream& sin) {
	sin >> specular[0] >> specular[1] >> specular[2];
}

void Material::parseShininess(std::stringstream& sin) {
	sin >> shininess;
}

void Material::parseTransparency(std::stringstream& sin) {
	GLfloat alpha;
	sin >> alpha;
	ambient[3] = diffuse[3] = specular[3] = alpha;
}

void Material::setAlpha(float alpha) {
	ambient[3] = diffuse[3] = specular[3] = alpha;
}

void Material::parseLine(std::stringstream& sin) {
	std::string s;
	sin >> s;
	if (s.compare("Ns") == 0) parseShininess(sin);
	else if (s.compare("Ka") == 0) parseAmbient(sin);
	else if (s.compare("Kd") == 0) parseDiffuse(sin);
	else if (s.compare("Ks") == 0) parseSpecular(sin);
	else if (s.compare("d") == 0) parseTransparency(sin);
}

void Material::loadMaterialData(std::string& filename) {
	std::ifstream ifile(filename);
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		parseLine(std::stringstream(line));
	}
}

const void Material::createMaterial(std::string& filename) {
	loadMaterialData(filename);
}

float* Material::getAmbient() {
	return ambient;
}

float* Material::getDiffuse() {
	return diffuse;
}

float* Material::getSpecular() {
	return specular;
}

float Material::getShininess() {
	return shininess;
}