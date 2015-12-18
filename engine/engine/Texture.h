//class for loading .tga textures
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "Dependencies/glew/glew.h"
#include "Dependencies/freeglut/freeglut.h"

#define TGA_RGB		2
#define TGA_A		3
#define TGA_RLE		10

typedef GLushort WORD;
typedef GLubyte byte;

typedef struct {
	int x;
	int y;
} Size;

typedef struct {
	int channels;
	Size size;
	unsigned char * data;
} Image;

class Texture {

private:

	GLuint texture_id;

public:

	Texture();

	void createTexture(char *fileName);
	Image *load(char *fileName);
	void draw();
};
