#version 330 core

const int MAX_POINT_LIGHTS = 2;                                                     
const int MAX_SPOT_LIGHTS = 2; 

struct Material {
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

uniform Material mat;
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

//lights
uniform int numPointLights;   
uniform PointLight pointLights;

//Fresnel
in float exRatio;
in vec3 exReflect;
in vec3 exRefractR;
in vec3 exRefractG;
in vec3 exRefractB;

out vec4 out_Color;

vec4 CalcPointLights(vec3 Normal, vec3 EyePos) {
	//Light
	vec4 LightPosition = vec4(0.0,0.0,-3.0, 1.0);
	vec4 LightAmbientColor = vec4(0.25, 0.25, 0.25, 1.0);
	vec4 LightDiffuseColor = vec4( 0.1, 0.1, 0.1, 1.0);
	vec4 LightAttenuation = vec4( 1.0, 0.045, 0.009, 1.0);
	float LightRange = 10.0;
	
	//light specific
	//for each light
	vec3 Lpos = vec3(ViewMatrix * LightPosition);
	vec3 L = Lpos - EyePos;
	float Ldistance = length(L);
	L = normalize(L);
	vec3 E = normalize(-EyePos);
	vec3 H = normalize(L + E);

	vec4 color = vec4(0.0);
	//if light in range - pointlight
	if(Ldistance < LightRange) {
		//Ambient
		vec4 ambient = LightAmbientColor * mat.ambient;
		float NdotL = max(dot(Normal,L), 0.0);
		//Diffuse
		vec4 diffuse = LightDiffuseColor * mat.diffuse * NdotL;
		//vec4 diffuse = vec4(NdotL,NdotL,NdotL, 0.5);
		//Specular
		vec4 specular = vec4(0.0);
		
		if(NdotL > 0.0) {
			// BLINN SPECULAR TERM (using half-vector H)
			float NdotH = max(dot(Normal,H), 0.0);
			float Blinn = pow(NdotH, mat.shininess * 0.0); // adjustment
			specular = mat.specular * Blinn;
		}
		//atennuation
		float attenuation = 1.0 / (
			LightAttenuation.x + 
			LightAttenuation.y * Ldistance + 
			LightAttenuation.z * pow(Ldistance, 2.0));	

		color = ambient + (diffuse + specular) * attenuation;
	}
	
	return color;
}

void main(void) {
	//Material
	vec3 MaterialEmissiveColor = vec3(0.15,0.15,0.15);
	vec3 emissive = MaterialEmissiveColor;
	
	//lighting
	vec3 V = vec3(DataIn.EyeVertexPos);
	vec3 N = normalize(DataIn.Normal);
	
	vec4 TotalLight = vec4(0,0,0,0);
	//point light
	/*for (int i = 0 ; i < gNumPointLights ; i++) {                                           
        //TotalLight += CalcPointLight(gPointLights[i], Normal);
		TotaLight += CalcPointLights(N, V);
    } */
	TotalLight += CalcPointLights(N, V);

	//Fresnel
	vec4 reflectColor;
	vec4 refractColor;
	reflectColor =   vec4(texture(tex_map, vec2(exReflect)));
	refractColor.r = (texture(tex_map, vec2(exRefractR))).r;
	refractColor.g = (texture(tex_map, vec2(exRefractG))).g;
	refractColor.b = (texture(tex_map, vec2(exRefractB))).b;
	refractColor.a = 0.2;
	vec4 colorFresnel = mix(refractColor, reflectColor, exRatio);
	
	out_Color = vec4(MaterialEmissiveColor, 1.0) + TotalLight;
		
	//vec4 texel = texture(tex_map, DataIn.Tex_Coord);
	//out_Color = texel;
	//out_Color = mat.diffuse;
	//out_Color = vec4(DataIn.Tex_Coord, 0.0, 0.0);
	//out_Color = DataIn.VertexPos;
	//out_Color = mat.specular;
	//out_Color = vec4(0.5, 0.5, 0.5, 1.0);
	out_Color = colorFresnel + TotalLight;
}