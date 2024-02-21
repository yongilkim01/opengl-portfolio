#version 330 core

in vec4 vertex_color;
in vec2 texture_coord;
out vec4 frag_color;

uniform sampler2D texture_obj;

void main()
{
    frag_color = texture(texture_obj, texture_coord);
}