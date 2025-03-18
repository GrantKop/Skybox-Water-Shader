#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 cameraMatrix;

void main()
{
    TexCoords = aPos;

    mat4 viewNoTranslation = mat4(mat3(cameraMatrix));
    
    vec4 pos = cameraMatrix * vec4(aPos, 1.0);
    
    gl_Position = pos.xyww;
}
