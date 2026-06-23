#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 LocalPos;
out vec3 esVertex;
out vec3 esNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // cameraView position
    vec4 eyePos = view * model * vec4(aPos, 1.0);
    esVertex = eyePos.xyz;

    // Normals for lighting calculation
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    esNormal = normalize(normalMatrix * aNormal);

    // Texture
    LocalPos = aPos;

    // Result
    gl_Position = projection * eyePos;
}