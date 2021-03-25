#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;

layout (location = 0) out vec3 f_Normal;
layout (location = 1) out vec3 f_FragPos;

layout (binding = 0) uniform ObjectData
{
    mat4 u_Model;
    mat4 u_Projection;
} Obj;

void main()
{
    gl_Position = Obj.u_Projection * Obj.u_Model * vec4(v_Position.xyz, 1.0);

    f_Normal = mat3(transpose(inverse(Obj.u_Model))) * v_Normal;
    f_FragPos = vec3(Obj.u_Model * vec4(v_Position.xyz, 1.0));
}
