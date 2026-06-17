#version 330 core

// Input vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// Uniforms for transformations
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Outputs to the fragment shader
out vec3 FragPos;
out vec3 Normal;

void main()
{
    // Calculate world-space position
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform normals (use inverse transpose for non-uniform scaling)
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    // Calculate final clip-space position
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
