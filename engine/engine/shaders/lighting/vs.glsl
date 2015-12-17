#version 330 core

in vec4 in_Position;
in vec4 in_Normal;
in vec4 in_TexCoord;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

uniform Camera {
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

out Data {
	vec4 EyeVertexPos;
	vec4 VertexPos;
	vec3 Normal;
	vec2 Tex_Coord;
} DataOut;

void main(void)
{
	mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
	mat4 ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	
	DataOut.EyeVertexPos = vec4(ModelViewMatrix * in_Position);
	DataOut.VertexPos = in_Position;
	DataOut.Normal = vec3(NormalMatrix * in_Normal);	//"downcast" from vec4 to vec3 
	DataOut.Tex_Coord = in_TexCoord.st;
	
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
}