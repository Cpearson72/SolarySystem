#version 330 core

uniform vec4 lightPosition; // eye space
uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;

in vec3 esVertex;
in vec3 esNormal;
in mat4 cameraView;

out vec4 fragColor;

void main()
{
    vec3 normal = normalize(esNormal);

    vec3 light;
    if(lightPosition.w == 0.0)
        light = normalize(lightPosition.xyz);
    else
        light = normalize(lightPosition.xyz - esVertex);

    vec3 viewDirection = normalize(vec3(cameraView[0].z, cameraView[1].z, cameraView[2].z));
    vec3 view = normalize(-viewDirection);

    vec3 reflectVec = reflect(-light, normal);

    float dotNL = max(dot(normal, light), 0.0);

    vec3 color = lightAmbient.rgb;
    color += lightDiffuse.rgb * dotNL;

    if(dotNL > 0.0)
    {
        float dotVR = max(dot(view, reflectVec), 0.0);
        color += lightSpecular.rgb * pow(dotVR, 32.0);
    }

    fragColor = vec4(color, 1.0);
}