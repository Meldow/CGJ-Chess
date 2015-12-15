#version 330 core

in vec4 in_Position;
in vec4 in_Normal;
in vec4 in_TexCoord;

uniform mat4 ModelMatrix;

uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

out Data {
	vec4 VertexPos;
	vec3 Normal;
	vec2 Tex_Coord;
} DataOut;

void main(void)
{
	DataOut.VertexPos = in_Position;
	DataOut.Tex_Coord = in_TexCoord.st;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
}