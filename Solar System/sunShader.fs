#version 330 core

// Inputs from the vertex shader
in vec3 FragPos;
in vec3 Normal;

// Output color
out vec4 FragColor;

// Material and light uniforms
uniform vec4 lightColor;

void main()
{
    FragColor = vec4(1.0, 242.0 / 255.0, 237.0 / 255.0, 1.0);
}
