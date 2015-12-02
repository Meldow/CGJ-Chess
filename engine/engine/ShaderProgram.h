#pragma once

#include "IDrawable.h"
#include "Dependencies/glew/glew.h"
#include <map>


class Matrix4;
class ShaderProgram {
public:
	GLuint _programId = -1;							//if program stays -1, we know it has errors
	std::map<const char*, GLint> _attributesId;		//contains ids of all uniforms
	std::map<const char*, GLint> _uniformsId;		//contains ids of all uniforms
	std::map<const char*, GLint> _uniformBlocksId;	//contains ids of all uniform blocksW
	
	//Constructors
	ShaderProgram();
	
	//Getters and setters
	void addShader(GLenum type, std::string shader);
	void addAttribute(const char* name, int index);
	void link();
	void addUniform(const char* name);
	void addUniformBlock(const char* name, GLuint UBO_BP);
	void create();
	void useProgram();
	GLuint getAttribute(char* name);
	GLuint getUniform(char* name);
	GLuint getUniformBlock(char* name);

	//Interfaces
	//void draw() override;	//?? quero passar argumento
	void draw(Matrix4 modelMatrix);

private:
	std::map<const char*, GLint>::iterator _attributesIdIterator;
	std::map<const char*, GLint>::iterator _uniformsIdIterator;
	std::map<const char*, GLint>::iterator _uniformBlocksIdIterator;
	GLenum shaders[];
	char* textFileRead(std::string fileName);
};