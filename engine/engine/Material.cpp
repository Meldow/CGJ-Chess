#include "Material.h"

Material::Material(std::string& filename)
{
	createMaterial(filename);
}

void Material::parseAmbient(std::stringstream& sin)
{
	sin >> _mat.ambient.kR >> _mat.ambient.kG >> _mat.ambient.kB;
}

void Material::parseDiffuse(std::stringstream& sin)
{
	sin >> _mat.diffuse.kR >> _mat.diffuse.kG >> _mat.diffuse.kB;
}

void Material::parseSpecular(std::stringstream& sin)
{
	sin >> _mat.specular.kR >> _mat.specular.kG >> _mat.specular.kB;
}

void Material::parseShininess(std::stringstream& sin)
{
	sin >> _mat.shininess;
}

void Material::parseTransparency(std::stringstream& sin)
{
	GLfloat alpha;
	sin >> alpha;
	_mat.ambient.kA = _mat.diffuse.kA = _mat.specular.kA = alpha;
}

void Material::parseLine(std::stringstream& sin)
{
	std::string s;
	sin >> s;
	if (s.compare("Ns") == 0) parseShininess(sin);
	else if (s.compare("Ka") == 0) parseAmbient(sin);
	else if (s.compare("Kd") == 0) parseDiffuse(sin);
	else if (s.compare("Ks") == 0) parseSpecular(sin);
	else if (s.compare("d") == 0) parseTransparency(sin);
}

void Material::loadMaterialData(std::string& filename)
{
	std::ifstream ifile(filename);
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		parseLine(std::stringstream(line));
	}
}

const void Material::createMaterial(std::string& filename)
{
	loadMaterialData(filename);
}

float* Material::getAmbient()
{
	float amb[] = {
		_mat.ambient.kR,
		_mat.ambient.kG,
		_mat.ambient.kB,
		_mat.ambient.kA
	};
	return amb;
}

float* Material::getDiffuse()
{
	float diffuse[] = {
		_mat.diffuse.kR,
		_mat.diffuse.kG,
		_mat.diffuse.kB,
		_mat.diffuse.kA
	};
	return diffuse;
}

float* Material::getSpecular()
{
	float spec[] = {
		_mat.specular.kR,
		_mat.specular.kG,
		_mat.specular.kB,
		_mat.specular.kA
	};
	return spec;
}

float* Material::getShininess()
{
	float shininess[] = { _mat.shininess };
	return shininess;
}
