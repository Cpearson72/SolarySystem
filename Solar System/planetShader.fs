#version 330 core

uniform vec4 lightPosition; // eye space
uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;

in vec3 esVertex;
in vec3 esNormal;
in mat4 cameraView;
in vec3 LocalPos;

out vec4 fragColor;

uniform sampler2D sphereMap;

void main()
{
    vec3 normal = normalize(esNormal);
    vec3 lightDir = normalize(lightPosition.xyz - esVertex);

   // Texture
    vec3 n = normalize(LocalPos);
    float u = atan(n.z, -n.x) / (2.0 * 3.14159265) + 0.5;
    float v = asin(n.y) / 3.14159265 + 0.5;
    vec2 TexCoord = vec2(u, v);
    vec4 texColor = texture(sphereMap, TexCoord);
    // Diffuse Lighting
    float dotNL = max(dot(normal, lightDir), 0.0);
    vec3 lighting = lightAmbient.rgb + (lightDiffuse.rgb * dotNL);
    // Result
    fragColor = vec4(texColor.rgb * lighting, texColor.a);
}