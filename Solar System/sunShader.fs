#version 330 core

// Inputs from the vertex shader
in vec3 FragPos;
in vec3 Normal;

// Output color
out vec4 FragColor;
uniform sampler2D sphereMap;

// Material and light uniforms
uniform vec4 lightColor;

void main()
{

    vec3 n = normalize(FragPos);
    float u = atan(n.z, -n.x) / (2.0 * 3.14159265) + 0.5;
    float v = asin(n.y) / 3.14159265 + 0.5;
    vec2 TexCoord = vec2(u, v);
    vec4 texColor = texture(sphereMap, TexCoord);

    FragColor = vec4(texColor.rgb, texColor.a);

}
