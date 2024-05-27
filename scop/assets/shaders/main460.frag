#version 460 core

out vec4 frag_color;

in vec2 var_tex;
in vec3 varying_col;
in vec3 varying_norm;
in vec3 frag_pos;

uniform float offset;
uniform vec3 light_src_pos;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    vec3 norm = normalize(varying_norm);
    vec3 dir = normalize(light_src_pos - frag_pos);
    vec3 ambient = vec3(0.2, 0.0, 0.0);

    float diff = max(dot(norm, dir), 0.0);
    vec3 diffuse = diff * vec3(1.0);
    //float inter = smoothstep(-1.0f, 1.0f, offset);
    //frag_color = mix(texture(texture0, var_tex), texture(texture1, var_tex), inter);
    //frag_color = frag_color * (vec4(varying_col, 1.0));
    frag_color = vec4(diffuse + ambient, 1.0) * vec4(1.0, 0.0, 0.0, 1.0);
} 