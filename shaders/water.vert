#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;

uniform float time;
uniform mat4 cameraMatrix;

void main() {
    vec3 pos = aPos;
    // Waves
    pos.y += sin(pos.x * 0.1 + time) * 0.5;
    pos.y += cos(pos.z * 0.1 + time) * 0.5;

    FragPos = pos;
    Normal = normalize(vec3(0.0, 1.0, 0.0));

    gl_Position = cameraMatrix * vec4(pos, 1.0);
}
