#version 450 core

layout (location = 0) in vec3 f_Normal;
layout (location = 1) in vec3 f_FragPos;

layout (location = 0) out vec4 o_Color;

layout (binding = 1) uniform RendererData
{
    vec3 u_LightDir;
    vec3 u_LightCol;
} r_Data;

void main()
{
    vec3 viewDir = normalize(-f_FragPos);
    vec3 reflectDir = reflect(-r_Data.u_LightDir, f_Normal);

    float ndotl = dot(f_Normal, r_Data.u_LightDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.1), 16);

    ndotl = max(0.1, ndotl);

    vec3 diffuse = r_Data.u_LightCol * ndotl;
    vec3 specular = vec3(spec);

    vec3 light = diffuse + specular;

    o_Color = vec4(light, 1.0) * vec4(1.0, 0.1, 0.1, 1.0);
}
