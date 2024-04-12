#version 460 core

out vec4 frag_color;

in vec3 varying_col;


void main()
{
    frag_color = vec4(varying_col, 1.0f);
} 