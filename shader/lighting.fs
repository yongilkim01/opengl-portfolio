#version 330 core
in vec3 position;
in vec3 normal;
in vec2 texture_coord;
out vec4 fragment_color;

uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 object_color;
uniform float ambient_strength;

uniform float specular_strength;
uniform float specular_shininess;
uniform vec3 view_position;

void main() {
    vec3 ambient = ambient_strength * light_color;
    
    vec3 light_direction = normalize(light_position - position); // 빛의 방향 = (광원 위치) - (각 픽셀의 3D 위치)
    vec3 pixel_normal = normalize(normal); // vertex shader 이후 보간된 normal 값이 unit 값인 것을 보장할 수 없으므로 재 정규화
    vec3 diffuse = max(dot(pixel_normal, light_direction), 0.0) * light_color;

    vec3 view_direction = normalize(view_position - position);
    vec3 reflect_direction = reflect(-light_direction, pixel_normal);
    //float spec = pow(max)

    vec3 result = (ambient + diffuse) * object_color;
    fragment_color = vec4(result, 1.0);
}