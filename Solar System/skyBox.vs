#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	mat4 rotationOnlyView = mat4(mat3(view)); 
    
    vec4 pos = projection * rotationOnlyView * vec4(aPos, 1.0);
    
    // Trick to force skybox to the background (Maximum Depth)
    gl_Position = pos.xyww;
	texCoords = vec3(aPos.x, aPos.y, -aPos.z);
}
	