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
uniform int mlwNumPointLights;   
//uniform PointLight mlwPointLights;

out vec4 out_Color;

vec4 CalcPointLights(vec3 Normal, vec3 EyePos) {
	//Light
	vec3 LightPosition = vec3(2.0,0.0,0.0);
	vec3 LightAmbientColor = vec3(0.2, 0.2, 0.2);
	vec3 LightDiffuseColor = vec3( 0.1, 0.1, 0.1);
	vec3 LightSpecularColor = vec3( 0.1, 0.1, 0.1);
	vec3 LightAttenuation = vec3( 0.1, 0.1, 0.1);
	float LightRange = 10.0;
	
	//light specific
	//for each light
	vec3 Lpos = vec3(ViewMatrix * vec4(LightPosition,1.0));
	vec3 L = Lpos - EyePos;
	float Ldistance = length(L);
	L = normalize(L);
	vec3 E = normalize(-EyePos);
	vec3 H = normalize(L + E);

	vec3 color = vec3(0.0);
	//if light in range - pointlight
	if(Ldistance < LightRange) {
		//Ambient
		vec3 ambient = LightAmbientColor * vec3(mat.ambient);
		float NdotL = max(dot(Normal,L), 0.0);
		//Diffuse
		vec3 diffuse = LightDiffuseColor * vec3(mat.diffuse) * NdotL;
		//Specular
		vec3 specular = vec3(0.0);
		
		if(NdotL > 0.0) {
			// BLINN SPECULAR TERM (using half-vector H)
			float NdotH = max(dot(Normal,H), 0.0);
			float Blinn = pow(NdotH, mat.shininess * 4.0); // adjustment
			specular = LightSpecularColor * vec3(mat.specular) * Blinn;
		}
		//atennuation
		float attenuation = 1.0 / (
			LightAttenuation.x + 
			LightAttenuation.y * Ldistance + 
			LightAttenuation.z * pow(Ldistance, 2.0));	

		color = ambient + (diffuse + specular) * attenuation;
	}
	
	return vec4(color, 1.0);
}

void main(void) {
	//Material
	vec3 MaterialEmissiveColor = vec3(0.1,0.1,0.1);
	
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
	
	//out_Color = vec4(MaterialEmissiveColor, 1.0) + TotalLight;
		
	vec4 texel = texture(tex_map, DataIn.Tex_Coord);
	out_Color = texel;
	//out_Color = mat.diffuse;
	//out_Color = vec4(DataIn.Tex_Coord, 0.0, 0.0);
	//out_Color = DataIn.VertexPos;
	//out_Color = mat.specular;
	//out_Color = vec4(0.5, 0.5, 0.5, 1.0);
}