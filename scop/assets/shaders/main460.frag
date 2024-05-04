#version 460 core

out vec4 frag_color;

in vec2 var_tex;
in vec3 varying_col;

uniform float offset;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    float inter = smoothstep(-1.0f, 1.0f, offset);
    frag_color = mix(texture(texture0, var_tex), texture(texture1, var_tex), inter) + 0.5*vec4(varying_col, 1.0);
} 