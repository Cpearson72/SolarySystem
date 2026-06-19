#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 esVertex;
out vec3 esNormal;
out mat4 cameraView;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 eyePos = view * model * vec4(aPos, 1.0);

    esVertex = eyePos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    esNormal = normalize(normalMatrix * aNormal);

    gl_Position = projection * eyePos;
    cameraView = view;
}