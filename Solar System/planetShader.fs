#version 330 core

uniform vec4 lightPosition; // eye space
uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;

in vec3 esVertex;
in vec3 esNormal;
in mat4 cameraView;

out vec4 fragColor;

uniform sampler2D sphereMap;

void main()
{
    vec3 normal = normalize(esNormal);
    vec3 lightDir = normalize(lightPosition.xyz - esVertex);

    vec3 viewDirection = normalize(-esVertex);

    vec3 reflectDir = reflect(-viewDirection, normal);
    float m = 2.0 * sqrt(reflectDir.x * reflectDir.x + 
                         reflectDir.y * reflectDir.y + 
                         (reflectDir.z + 1.0) * (reflectDir.z + 1.0));
    
    vec2 texCoords;
    texCoords.s = (reflectDir.x / m) + 0.5;
    texCoords.t = (reflectDir.y / m) + 0.5;

    vec4 texColor = texture(sphereMap, texCoords);
    float dotNL = max(dot(normal, lightDir), 0.0);
    vec3 lighting = lightAmbient.rgb + (lightDiffuse.rgb * dotNL);
    fragColor = vec4(texColor.rgb * lighting, texColor.a);
}