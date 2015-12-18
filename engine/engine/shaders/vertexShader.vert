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

out float exRatio;
out vec3 exReflect;
out vec3 exRefractR;
out vec3 exRefractG;
out vec3 exRefractB;

const float ETA = 0.96; // Glass Reflectance coefficient = 4%
const float ETA_R = ETA; // No aberration if all components equal
const float ETA_G = ETA;
const float ETA_B = ETA;

const float FRESNEL_POWER = 5.0;
const float F = ((1.0 - ETA) * (1.0 - ETA)) /
                ((1.0 + ETA) * (1.0 + ETA));

vec3 toWorldSpace(vec3 v) 
{
	return vec3(ModelMatrix * vec4(v,1.0));
}

void main(void)
{
	mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
	mat4 ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	
	DataOut.EyeVertexPos = vec4(ModelViewMatrix * in_Position);
	DataOut.VertexPos = in_Position;
	DataOut.Normal = vec3(NormalMatrix * in_Normal);	//"downcast" from vec4 to vec3 
	DataOut.Tex_Coord = in_TexCoord.st;

	mat4 invModelViewMatrix = inverse(ViewMatrix * ModelMatrix);
	vec3 cameraPosition = vec3(invModelViewMatrix[3])/invModelViewMatrix[3][3];
	vec3 V = vec3(in_Position);
	vec3 I = normalize(cameraPosition - V);
	vec3 N = normalize(vec3(in_Normal));

	// Fresnel term, reflection and refraction	
	exRatio = F + (1.0 - F) * pow((1.0 - dot(I,N)), FRESNEL_POWER);
	exReflect = toWorldSpace(reflect(-I, N));
	// Chromatic aberration effect
	exRefractR = toWorldSpace(refract(-I, N, ETA_R));
	exRefractG = toWorldSpace(refract(-I, N, ETA_G));
	exRefractB = toWorldSpace(refract(-I, N, ETA_B));
	
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
}