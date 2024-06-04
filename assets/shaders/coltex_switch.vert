#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 4) in vec3 col;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;

out vec3 varying_local_frag_pos;

out vec3 default_color;

vec3 get_default_color()
{
    vec3 ret = vec3(1.0);
    switch (gl_VertexID % 18)
    {
        case 0: case 1: case 2:
            ret = vec3(0.0);
            break;
        case 3: case 4: case 5:
            ret = vec3(0.1);
            break;
        case 6: case 7: case 8:
            ret = vec3(0.2);
            break;
        case 9: case 10: case 11:
            ret = vec3(0.3);
            break;
        case 12: case 13: case 14:
            ret = vec3(0.4);
            break;
        case 15: case 16: case 17:
            ret = vec3(0.5);
            break;
    };
    return ret;
}

void main()
{
    varying_local_frag_pos = pos;

    default_color = get_default_color();
    gl_Position = proj * view * model * vec4(pos, 1.0);
}