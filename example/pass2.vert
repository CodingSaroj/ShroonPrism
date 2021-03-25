#version 450 core

layout (location = 0) in vec3 v_Vertices;

layout (location = 0) out vec2 f_UVs;

layout (binding = 0) uniform ObjectData
{
    mat4 u_Model;
} r_ObjectData;

void main()
{
    gl_Position = vec4(v_Vertices, 1.0);
    f_UVs = vec2(v_Vertices.x, v_Vertices.y);
}
