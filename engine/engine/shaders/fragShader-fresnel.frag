#version 330 core

const int MAX_POINT_LIGHTS = 2;                                                     
const int MAX_SPOT_LIGHTS = 2; 

struct Material {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	float shininess;
};
struct BaseLight {                                                                                   
    vec3 Color;                                                                   
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
};                                                                                  
struct DirectionalLight {                                                                                   
    BaseLight Base;                                                                 
    vec3 Direction;                                                                 
};                                                                                  
struct Attenuation {                                                                                   
    float Constant;                                                                 
    float Linear;                                                                   
    float Exp;                                                                      
};                                                                                  
struct PointLight {                                                                                           
    BaseLight Base;                                                                         
    vec3 Position;                                                                          
    vec3 Atten;                                                                      
	float Range;
};                                                                                          
struct SpotLight {                                                                                           
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
uniform PointLight pointLights[MAX_POINT_LIGHTS];

//Fresnel
in float exRatio;
in vec3 exReflect;
in vec3 exRefractR;
in vec3 exRefractG;
in vec3 exRefractB;

out vec4 out_Color;

vec4 CalcPointLights(PointLight pointlight) {
	vec3 V = vec3(DataIn.VertexPos);		//incorrect world placement		light does not move
	//vec3 V = vec3(DataIn.EyeVertexPos);	//correct world placement		light rotates
	vec3 N = normalize(DataIn.Normal);
	vec3 Lpos = vec3(ViewMatrix * vec4(pointlight.Position,1.0));
	vec3 L = Lpos - V;
	float Ldistance = length(L);
	L = normalize(L);
	vec3 E = normalize(-V);
	vec3 H = normalize(L + E); /**/

	
	if(Ldistance < pointlight.Range) {
		vec3 ambient = pointLights[0].Base.AmbientIntensity * pointLights[0].Base.Color * vec3(mat.ambient);
		float NdotL = max(dot(N,L), 0.0);
		vec3 diffuse = pointlight.Base.DiffuseIntensity * pointlight.Base.Color * vec3(mat.diffuse) * NdotL;
		vec3 specular = vec3(0.0);
		//problems with specular
		if(NdotL > 0.0) {
			// BLINN SPECULAR TERM (using half-vector H)
			float NdotH = max(dot(N,H), 0.0);
			float Blinn = pow(NdotH, mat.shininess * 4.0); // adjustment
			specular = vec3(mat.specular) * Blinn;
		}
    
		float attenuation = 1.0 / (
			pointlight.Atten.x + 
			pointlight.Atten.y * Ldistance + 
			pointlight.Atten.z * pow(Ldistance, 2.0));	
	
		return vec4(ambient + (diffuse)* attenuation,1.0);
		//return vec4(specular, 1.0);
		//color = vec4(ambient + (diffuse + specular) * attenuation, 1.0);
	}
	return vec4(0.0,0.0,0.0,0.0);
}

void main(void) {
	//lighting
	vec4 TotalLight = vec4(0,0,0,0);
	for (int i = 0 ; i < numPointLights ; i++) {                                           
		TotalLight += CalcPointLights(pointLights[i]);
	}

	//Fresnel
	vec4 reflectColor;
	vec4 refractColor;
	reflectColor =   vec4(texture(tex_map, vec2(exReflect)));
	refractColor.r = (texture(tex_map, vec2(exRefractR))).r;
	refractColor.g = (texture(tex_map, vec2(exRefractG))).g;
	refractColor.b = (texture(tex_map, vec2(exRefractB))).b;
	refractColor.a = 0.2;
	vec4 colorFresnel = mix(refractColor, reflectColor, exRatio);
	
	//vec4 texel = texture(tex_map, DataIn.Tex_Coord);
	//out_Color = texel;
	//out_Color = mat.diffuse;
	//out_Color = vec4(DataIn.Tex_Coord, 0.0, 0.0);
	//out_Color = DataIn.VertexPos;
	//out_Color = mat.specular;
	//out_Color = vec4(0.5, 0.5, 0.5, 1.0);
	out_Color = colorFresnel + TotalLight;
}