#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texture_coord;

uniform mat4 transform;
uniform mat4 model_transform;

out vec3 normal;
out vec2 texture_coord;
out vec3 position;

void main() {
    gl_Position = transform * vec4(a_position, 1.0);
    normal = (transpose(inverse(model_transform)) * vec4(a_normal, 0.0)).xyz;
    texture_coord = a_texture_coord;
    position = (model_transform * vec4(a_position, 1.0)).xyz;
}