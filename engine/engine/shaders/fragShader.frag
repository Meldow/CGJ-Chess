#version 330 core

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	float shininess;
};

uniform Materials mat;
uniform sampler2D tex_map;

in Data {
	vec4 VertexPos;
	vec3 Normal;
	vec2 Tex_Coord;
} DataIn;

out vec4 out_Color;

void main(void)
{
	vec4 texel = texture(tex_map, DataIn.Tex_Coord);
	
	//out_Color = texel;
	out_Color = mat.diffuse;
	//out_Color = vec4(DataIn.Tex_Coord, 0.0, 0.0);
	//out_Color = DataIn.VertexPos;
	//out_Color = mat.specular;
	//out_Color = vec4(0.5, 0.5, 0.5, 1.0);
}