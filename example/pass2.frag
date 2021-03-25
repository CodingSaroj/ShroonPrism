#version 450 core

layout (location = 0) in vec2 f_UVs;

layout (location = 0) out vec4 o_Color;

layout (binding = 0) uniform sampler2D u_Texture;

void main()
{
    o_Color = texture(u_Texture, f_UVs);
}
