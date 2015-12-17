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

//Fresnel
in float exRatio;
in vec3 exReflect;
in vec3 exRefractR;
in vec3 exRefractG;
in vec3 exRefractB;

vec4 ambientReflection;
vec4 diffuseReflection;
vec4 specularReflection;
float fresnel;
vec4 reflectedColor;

out vec4 out_Color;

vec4 CalcPointLights(vec3 Normal, vec3 EyePos) {
	//Light
	vec3 LightPosition = vec3(2.0,0.0,0.0);
	vec4 LightAmbientColor = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 LightDiffuseColor = vec4(0.1, 0.1, 0.1, 1.0);
	vec4 LightSpecularColor = vec4(0.1, 0.1, 0.1, 1.0);
	vec4 LightAttenuation = vec4(0.1, 0.1, 0.1, 1.0);
	float LightRange = 10.0;
	
	//light specific
	//for each light
	vec3 Lpos = vec3(ViewMatrix * vec4(LightPosition,1.0));
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
		//Specular
		vec4 specular = vec4(0.0);
		
		if(NdotL > 0.0) {
			// BLINN SPECULAR TERM (using half-vector H)
			float NdotH = max(dot(Normal,H), 0.0);
			float Blinn = pow(NdotH, mat.shininess * 4.0); // adjustment
			specular = LightSpecularColor * mat.specular * Blinn;
		}
		//atennuation
		float attenuation = 1.0 / (
			LightAttenuation.x + 
			LightAttenuation.y * Ldistance + 
			LightAttenuation.z * pow(Ldistance, 2.0));	

		color = ambient + (diffuse + specular) * attenuation;





	//Interpolated normal, light direction and view direction
	 vec3 N = normalize(DataIn.Normal);

	 //Fresnel approximation
	 float base = 1-dot(E, H);
	 float exp = pow(base, 5);
	 fresnel = exp+pow((1.0f-(1.0f/1.31f)), 2)/pow((1.0f+(1.0f/1.31f)), 2)*(1.0-exp);

	 //Blinn-phong reflection model
	 ambientReflection = mat.ambient;
	 diffuseReflection = mat.diffuse*clamp(dot(L, N), 0, 1);
	 specularReflection = mat.specular*max(0.0,pow(dot(N, H), 80));

	 //Final color of the fragment
	 reflectedColor =  ambientReflection + diffuseReflection + fresnel*specularReflection;


	}
	
	return color;
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

	//Fresnel
	vec3 reflectColor, refractColor;
	reflectColor = vec3(texture(tex_map, vec2(exReflect)));
	refractColor.r = (texture(tex_map, vec2(exRefractR))).r;
	refractColor.g = (texture(tex_map, vec2(exRefractG))).g;
	refractColor.b = (texture(tex_map, vec2(exRefractB))).b;
	vec3 colorFresnel = mix(refractColor, reflectColor, exRatio);
	
	//out_Color = vec4(MaterialEmissiveColor, 1.0) + TotalLight;
		
	//vec4 texel = texture(tex_map, DataIn.Tex_Coord);
	//out_Color = texel;
	//out_Color = mat.diffuse;
	//out_Color = vec4(DataIn.Tex_Coord, 0.0, 0.0);
	//out_Color = DataIn.VertexPos;
	//out_Color = mat.specular;
	//out_Color = vec4(0.5, 0.5, 0.5, 1.0);
	//out_Color = vec4(colorFresnel,1.0);
	out_Color = reflectedColor;

}