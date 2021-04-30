#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_UV;
layout (location = 2) in vec3 v_Normal;

layout (location = 0) out vec2 f_UV;
layout (location = 1) out vec3 f_Normal;
layout (location = 2) out vec3 f_FragPos;

layout (binding = 1, std140) uniform ObjectData
{
    mat4 u_Model;
    mat4 u_Projection;
    float u_Roughness;
    float u_Specular;
};

void main()
{
    vec4 worldPos = u_Model * vec4(v_Position.xyz, 1.0);

    gl_Position = u_Projection * worldPos;

    f_UV = v_UV;
    f_Normal = mat3(transpose(inverse(u_Model))) * v_Normal;
    f_FragPos = vec3(worldPos);
}
