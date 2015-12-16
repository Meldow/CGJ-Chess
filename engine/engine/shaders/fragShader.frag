#version 330 core

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	float shininess;
};

uniform Materials mat;
uniform sampler2D tex_map;

uniform Camera
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

in Data {
	vec4 EyeVertexPos;
	vec4 VertexPos;
	vec3 Normal;
	vec2 Tex_Coord;
} DataIn;

out vec4 out_Color;

void main(void) {
	/*
	//Material
	vec3 MaterialEmissiveColor = vec3(0.1,0.1,0.1);
	vec3 MaterialAmbientColor = vec3(mat.ambient);
	vec3 MaterialDiffuseColor = vec3(mat.diffuse);
	vec3 MaterialSpecularColor = vec3(mat.specular);
	float MaterialShininess = mat.shininess;
	
	//Light
	vec3 LightPosition = vec3(2.0,2.0,2.0);
	vec3 LightAmbientColor = vec3(0.2, 0.2, 0.2);
	vec3 LightDiffuseColor = vec3( 0.1, 0.1, 0.1);
	vec3 LightSpecularColor = vec3( 0.1, 0.1, 0.1);
	vec3 LightAttenuation = vec3( 0.1, 0.1, 0.1);
	float LightRange = 10.0;
	
	//lighting
	vec3 V = vec3(DataIn.EyeVertexPos);
	vec3 N = normalize(DataIn.Normal);
	vec3 Lpos = vec3(ViewMatrix * vec4(LightPosition,1.0));
	vec3 L = Lpos - V;
	float Ldistance = length(L);
	L = normalize(L);
	vec3 E = normalize(-V);
	vec3 H = normalize(L + E);

	vec3 emissive = MaterialEmissiveColor;

	vec3 color = vec3(0.0);
	//if light in range - pointlight
	if(Ldistance < LightRange) {
		//Ambient
		vec3 ambient = LightAmbientColor * MaterialAmbientColor;
		float NdotL = max(dot(N,L), 0.0);
		//Diffuse
		vec3 diffuse = LightDiffuseColor * MaterialDiffuseColor * NdotL;
		//Specular
		vec3 specular = vec3(0.0);
		
		if(NdotL > 0.0) {
			// BLINN SPECULAR TERM (using half-vector H)
			float NdotH = max(dot(N,H), 0.0);
			float Blinn = pow(NdotH, MaterialShininess * 4.0); // adjustment
			specular = LightSpecularColor * MaterialSpecularColor * Blinn;
		}
		//atennuation
		float attenuation = 1.0 / (
			LightAttenuation.x + 
			LightAttenuation.y * Ldistance + 
			LightAttenuation.z * pow(Ldistance, 2.0));	

		color = ambient + (diffuse + specular) * attenuation;
	}

	out_Color = vec4(emissive + color, 1.0);
	*/
	
	vec4 texel = texture(tex_map, DataIn.Tex_Coord);
	out_Color = texel;
	//out_Color = mat.diffuse;
	//out_Color = vec4(DataIn.Tex_Coord, 0.0, 0.0);
	//out_Color = DataIn.VertexPos;
	//out_Color = mat.specular;
	//out_Color = vec4(0.5, 0.5, 0.5, 1.0);
}