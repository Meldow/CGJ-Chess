#include "Mesh.h"

Mesh::Mesh(std::string& filename) {
	createMesh(filename);
	createBufferObjects();
}

void Mesh::parseVertex(std::stringstream& sin) {
	Vertex v;
	sin >> v.x >> v.y >> v.z;
	vertexData.push_back(v);
}

void Mesh::parseTexcoord(std::stringstream& sin) {
	Texcoord t;
	sin >> t.u >> t.v;
	texcoordData.push_back(t);
}

void Mesh::parseNormal(std::stringstream& sin) {
	Normal n;
	sin >> n.nx >> n.ny >> n.nz;
	normalData.push_back(n);
}

void Mesh::parseFace(std::stringstream& sin) {
	std::string token;
	for (int i = 0; i < 3; i++) {
		std::getline(sin, token, '/');
		if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
		std::getline(sin, token, '/');
		if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
		std::getline(sin, token, ' ');
		if (token.size() > 0) normalIdx.push_back(std::stoi(token));
	}
}

void Mesh::parseLine(std::stringstream& sin) {
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("f") == 0) parseFace(sin);
}

void Mesh::loadMeshData(std::string& filename) {
	std::ifstream ifile(filename);
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		parseLine(std::stringstream(line));
	}
	texCoordsLoaded = (texcoordIdx.size() > 0);
	normalsLoaded = (normalIdx.size() > 0);
}

void Mesh::processMeshData() {
	for (unsigned int i = 0; i < vertexIdx.size(); i++) {
		unsigned int vi = vertexIdx[i];
		Vertex v = vertexData[vi - 1];
		vertices.push_back(v);
		if (texCoordsLoaded) {
			unsigned int ti = texcoordIdx[i];
			Texcoord t = texcoordData[ti - 1];
			texCoords.push_back(t);
		}
		if (normalsLoaded) {
			unsigned int ni = normalIdx[i];
			Normal n = normalData[ni - 1];
			normals.push_back(n);
		}
	}
}

void Mesh::freeMeshData() {
	vertexData.clear();
	texcoordData.clear();
	normalData.clear();
	vertexIdx.clear();
	texcoordIdx.clear();
	normalIdx.clear();
}

const void Mesh::createMesh(std::string& filename) {
	loadMeshData(filename);
	processMeshData();
	freeMeshData();
}

void Mesh::createBufferObjects() {
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(1, &VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	if (texCoordsLoaded) {
		glGenBuffers(1, &VboTexcoords);
		glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(Texcoord), &texCoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(TEXCOORDS);
		glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Texcoord), 0);
	}
	if (normalsLoaded) {
		glGenBuffers(1, &VboNormals);
		glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Normal), &normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(NORMALS);
		glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::destroyBufferObjects() {
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(NORMALS);
	glDeleteBuffers(1, &VboVertices);
	glDeleteBuffers(1, &VboTexcoords);
	glDeleteBuffers(1, &VboNormals);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::bindVertexArray() {
	glBindVertexArray(VaoId);
}

void Mesh::draw() {
	bindVertexArray();
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}