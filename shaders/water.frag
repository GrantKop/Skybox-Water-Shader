#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D screenTexture;
uniform mat4 cameraMatrix;
uniform vec3 viewPos;

vec2 computeScreenUV(vec3 worldPos) {
    vec4 clipSpace = cameraMatrix * vec4(worldPos, 1.0);
    vec3 ndc = clipSpace.xyz / clipSpace.w;
    vec2 screenUV = ndc.xy * 0.5 + 0.5;

    return screenUV;
}

void main() {
    vec3 viewDir = normalize(FragPos - viewPos);
    vec3 normal = normalize(Normal); 

    vec3 reflectionDir = reflect(viewDir, normal);
    vec3 reflectedPos = FragPos + reflectionDir * 5.0;

    vec2 screenUV = computeScreenUV(reflectedPos);

    screenUV.y = 1.0 - screenUV.y;

    screenUV = clamp(screenUV, vec2(0.01, 0.01), vec2(0.99, 0.99));

    FragColor = texture(screenTexture, screenUV);
}
