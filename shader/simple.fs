#version 330 core

out vec4 frag_color;
in vec3 our_color;

void main()
{
    frag_color = vec4(our_color, 1.0);
    //frag_color = vec4(1.0, 0.0, 0.0, 1.0);
}