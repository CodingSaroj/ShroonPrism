#version 450 core

layout (location = 0) in vec2 f_UV;
layout (location = 1) in vec3 f_Normal;
layout (location = 2) in vec3 f_FragPos;

layout (location = 0) out vec4 o_Color;

layout (binding = 0, std140) uniform RendererData
{
    vec4 u_LightDir;
    vec4 u_LightCol;
};

layout (binding = 1, std140) uniform ObjectData
{
    mat4 u_Model;
    mat4 u_Projection;
    float u_Roughness;
    float u_Specular;
};

layout (location = 0) uniform sampler2D u_Albedo;

void main()
{

    float ndotl = max(dot(f_Normal, u_LightDir.xyz), 0.2);
    vec3 diffuse = u_LightCol.xyz * ndotl;

    vec3 viewDir = normalize(-f_FragPos);
    vec3 reflectDir = reflect(-u_LightDir.xyz, f_Normal);

    float vdotr = max(dot(viewDir, reflectDir), 0.0);
    vec3 specular = u_Specular * u_LightCol.xyz * vec3(pow(vdotr, max((1.0 - u_Roughness) * 256.0, 1.0)));

    vec3 light = diffuse + specular;

    o_Color = vec4(light, 1.0) * texture(u_Albedo, f_UV);
}
