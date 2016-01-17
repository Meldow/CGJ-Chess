#version 330 core

struct Material {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    float shininess;
};

uniform Material mat;
uniform sampler3D tex_map;

uniform mat4 ModelMatrix;
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

const vec3 MarbleColor = vec3(0.15,0.15,0.15);
const vec3 VeinColor = vec3(0.05,0.05,0.05);

out vec4 out_Color;

void main(void) {
    
    vec3 wcPosition = vec3(ModelMatrix * DataIn.VertexPos);
    vec3 p = wcPosition * 0.5 + 0.5;
    float noise = texture(tex_map, p).r * 0.5 + 0.5;
    
    float intensity = clamp(noise, 0.0, 1.0);
    vec3 color = mix(MarbleColor, VeinColor, intensity);
    
    out_Color = vec4(color, 1.0);
}