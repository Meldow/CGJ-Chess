#version 330 core

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	float shininess;
};

struct BaseLight                                                                    
{                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
};                                                                                  
                                                                                    
struct DirectionalLight                                                             
{                                                                                   
    BaseLight Base;                                                                 
    vec3 Direction;                                                                 
};                                                                                  
                                                                                    
struct Attenuation                                                                  
{                                                                                   
    float Constant;                                                                 
    float Linear;                                                                   
    float Exp;                                                                      
};                                                                                  
                                                                                    
struct PointLight                                                                           
{                                                                                           
    BaseLight Base;                                                                         
    vec3 Position;                                                                          
    Attenuation Atten;                                                                      
};                                                                                          
                                                                                            
struct SpotLight                                                                            
{                                                                                           
    PointLight Base;                                                                        
    vec3 Direction;                                                                         
    float Cutoff;                                                                           
};

uniform Materials mat;
uniform sampler2D tex_map;

uniform Camera {
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

in Data {
	vec4 EyeVertexPos;
	vec4 VertexPos;
	vec3 Normal;
	vec2 Tex_Coord;
} DataIn;

in float exRatio;
in vec3 exReflect;
in vec3 exRefractR;
in vec3 exRefractG;
in vec3 exRefractB;

out vec4 out_Color;

void main(void) {
	//Material
	vec3 MaterialEmissiveColor = vec3(0.1,0.1,0.1);
	vec4 MaterialAmbientColor = mat.ambient;
	vec4 MaterialDiffuseColor = mat.diffuse;
	vec4 MaterialSpecularColor = mat.specular;
	float MaterialShininess = mat.shininess;
	
	
	//First light Model
	

	//Light
	vec3 LightPosition = vec3(2.0,2.0,2.0);
	vec4 LightAmbientColor = vec4(0.2, 0.2, 0.2,1.0);
	vec4 LightDiffuseColor = vec4( 0.1, 0.1, 0.1,1.0);
	vec4 LightSpecularColor = vec4( 0.1, 0.1, 0.1,1.0);
	vec4 LightAttenuation = vec4( 0.1, 0.1, 0.1,1.0);
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

	vec4 emissive = vec4(MaterialEmissiveColor,1.0);

	vec4 color = vec4(0.0);
	//if light in range - pointlight
	if(Ldistance < LightRange) {
		//Ambient
		vec4 ambient = LightAmbientColor * MaterialAmbientColor;
		float NdotL = max(dot(N,L), 0.0);
		//Diffuse
		vec4 diffuse = LightDiffuseColor * MaterialDiffuseColor * NdotL;
		//Specular
		vec4 specular = vec4(0.0);
		
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

	//Fresnel
	vec3 reflectColor, refractColor;
	reflectColor = vec3(texture(tex_map, vec2(exReflect)));
	refractColor.r = (texture(tex_map, vec2(exRefractR))).r;
	refractColor.g = (texture(tex_map, vec2(exRefractG))).g;
	refractColor.b = (texture(tex_map, vec2(exRefractB))).b;

	vec3 colorF = mix(refractColor, reflectColor, exRatio);

	out_Color = vec4(emissive + color);
	
	//vec4 texel = texture(tex_map, DataIn.Tex_Coord);
	//out_Color = texel;
	//out_Color = mat.diffuse;
	//out_Color = vec4(DataIn.Tex_Coord, 0.0, 0.0);
	//out_Color = DataIn.VertexPos;
	//out_Color = mat.specular;
	//out_Color = vec4(colorF,1.0) * mat.specular;
	//out_Color = vec4(0.5, 0.5, 0.5, 1.0);
}