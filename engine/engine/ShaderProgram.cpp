#include "ShaderProgram.h"
#include "Matrices.h"
#include <iostream>
#include <vector>

ShaderProgram::ShaderProgram() {
	_programId = glCreateProgram();
}

void ShaderProgram::addShader(GLenum type, std::string shader) {
	char *s = NULL;
	s = textFileRead(shader);

	if (s != NULL) {
		//////////////////////////////////////////////////////////////////////// Debug
		GLint result = GL_FALSE;
		int logLength;
		////////////////////////////////////////////////////////////////////////

		const char * ss = s;
		shaders[type] = glCreateShader(type);
		glShaderSource(shaders[type], 1, &ss, NULL);
		glCompileShader(shaders[type]);

		//////////////////////////////////////////////////////////////////////// Debug
		glGetShaderiv(shaders[type], GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaders[type], GL_INFO_LOG_LENGTH, &logLength);

		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> programError((logLength > 1) ? logLength : 1);
		glGetProgramInfoLog(_programId, logLength, NULL, &programError[0]);
		std::cout << &programError[0] << std::endl;

		if (programError.size() > 1)std::getchar();	//
		////////////////////////////////////////////////////////////////////////

		glAttachShader(_programId, shaders[type]);
		free(s);
	} else {
		//std::cout << "\nCould not find shader: " << shader;	//??
		std::cout << "\nCould not find shader.";
	}
}

void ShaderProgram::addAttribute(const char* name, int index) {
	glBindAttribLocation(_programId, index, name);
	_attributesId.insert(std::pair<const char*, GLint>(name, -1));
}

void ShaderProgram::link() {
	glLinkProgram(_programId);

	//////////////////////////////////////////////////////////////////////// Debug
	GLint result = GL_FALSE;
	int logLength;

	glGetProgramiv(_programId, GL_LINK_STATUS, &result);
	glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(_programId, logLength, NULL, &programError[0]);
	std::cout << &programError[0] << std::endl;
}

void ShaderProgram::addUniform(const char* name) {
	_uniformsId.insert(std::pair<const char*, GLint>(name, -1));
}

void ShaderProgram::addUniformBlock(const char* name, GLuint UBO_BP) {
	_uniformBlocksId.insert(std::pair<const char*, GLint>(name, UBO_BP));
}

void ShaderProgram::create() {
	for (_attributesIdIterator = _attributesId.begin(); _attributesIdIterator != _attributesId.end(); ++_attributesIdIterator) {
		_attributesId[_attributesIdIterator->first] = glGetAttribLocation(_programId, _attributesIdIterator->first);

		if (_attributesIdIterator->second == -1) {
			std::cout << "\nWarning :: Cannot find attribute :: " << _attributesIdIterator->first;
		}
	}
	for (_uniformsIdIterator = _uniformsId.begin(); _uniformsIdIterator != _uniformsId.end(); ++_uniformsIdIterator) {
		_uniformsId[_uniformsIdIterator->first] = glGetUniformLocation(_programId, _uniformsIdIterator->first);

		if (_uniformsIdIterator->second == -1) {
			std::cout << "\nWarning :: Cannot find uniform :: " << _uniformsIdIterator->first;
		}
	}
	//gets all the correct values to the hash tables uniform blocks
	for (_uniformBlocksIdIterator = _uniformBlocksId.begin(); _uniformBlocksIdIterator != _uniformBlocksId.end(); ++_uniformBlocksIdIterator) {
		_uniformBlocksId[_uniformBlocksIdIterator->first] = glGetUniformBlockIndex(_programId, _uniformBlocksIdIterator->first);

		if (_uniformBlocksIdIterator->second == -1) {
			std::cout << "\nWarning :: Cannot find uniform block :: " << _uniformBlocksIdIterator->first;
		}
	}

	//Camera* camera = SceneManager::instance()->_activeCamera;
	//if (camera)
	//glUniformBlockBinding(_pId, camera->UboId, camera->UBO_BP);
}

void ShaderProgram::useProgram() {
	glUseProgram(_programId);
}

GLuint ShaderProgram::getAttribute(char* name) {
	return _attributesId[name];
}

GLuint ShaderProgram::getUniform(char* name) {
	return _uniformsId[name];
}

GLuint ShaderProgram::getUniformBlock(char* name) {
	return _uniformBlocksId[name];
}

void ShaderProgram::draw(Matrix4 ModelMatrix) {
	useProgram();
	//hammer
	glUniformMatrix4fv(getUniform("ModelMatrix"), 1, GL_FALSE, ModelMatrix.get());

	//will need to get all other uniforms

}

char* ShaderProgram::textFileRead(std::string fileName) {
	FILE *fp;
	char *content = NULL;

	int count = 0;

	if (fileName != "") {
		fp = fopen(fileName.c_str(), "rt");

		if (fp != NULL) {

			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}