#version 330 core

// Inputs from the vertex shader
in vec3 FragPos;
in vec3 Normal;

// Output color
out vec4 FragColor;

// Material and light uniforms
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // 1. Ambient Lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // 2. Diffuse Lighting (Lambertian reflection)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    
    // Compute dot product of normal and light direction clamped to [0, 1]
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // 3. Final color
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
