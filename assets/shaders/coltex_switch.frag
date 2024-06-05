#version 460 core

struct material
{
    float Ns;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Ke;
    float Ni;
    float d;
    int illum;

    sampler2D diffuse_map;
    bool diffuse_map_enabled;

    sampler2D bump_map;
    bool bump_map_enabled;

    sampler2D specular_map;
    bool specular_map_enabled;

    sampler2D default_map;
    bool default_map_enabled;
};

out vec4 frag_color;

in vec3 varying_local_frag_pos;
in vec3 default_color;

uniform material mat;
uniform float trigger;

void main()
{
    vec2 test_tex = vec2(varying_local_frag_pos.z, -varying_local_frag_pos.y);

    vec4 col1 = texture(mat.default_map, test_tex);
    vec4 col2 = vec4(default_color, 1.0);
    frag_color = mix(col1, col2, trigger);
}