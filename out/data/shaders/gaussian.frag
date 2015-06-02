#version 330

in vec3 vertexNormalCamSpace;
in vec3 cameraSpacePosition;
in vec2 tc;
in vec4 color;

out vec4 outputColor;

// Make all uniform blocks follow this layout
layout(std140) uniform;

struct PerLight
{
    vec4    lightWorldPos;
    vec4    lightIntensity;
    bool    isOn;
};

const int numberOfLights = 8;

uniform Light
{
    vec4        ambientIntensity;
    float       lightAttenuation;
    float       maxIntensity;
    float       gammaInv;
    PerLight    lights[numberOfLights];
} Lgt;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform sampler2D diffuseTexture;

float shininessFactor = 0.1;
vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0);

vec4 calcLighting(in PerLight lightData)
{
    vec3 lightCamSpace = vec3(viewMatrix * vec4(lightData.lightWorldPos.xyz, 1.0));
    float distToLight = length(lightCamSpace - cameraSpacePosition);
    vec3 lightDir = normalize(lightCamSpace - cameraSpacePosition);

    vec3 surfaceNormal = normalize(vertexNormalCamSpace);
    float cosAngIncidence = dot(surfaceNormal, lightDir);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);
    
    vec3 viewDir = normalize(-cameraSpacePosition);
    
    vec3 halfAngle = normalize(lightDir + viewDir);
    float angleNormalHalf = acos(dot(halfAngle, surfaceNormal));
    float exponent = angleNormalHalf / shininessFactor;
    exponent = -(exponent * exponent);
    float gaussianTerm = exp(exponent);

    gaussianTerm = cosAngIncidence != 0.0 ? gaussianTerm : 0.0;

    vec4 attenIntensity = lightData.lightIntensity / (1.0 + Lgt.lightAttenuation + distToLight * distToLight);    

    return (color * attenIntensity * cosAngIncidence) +
        (specularColor * attenIntensity * gaussianTerm);
    // return (texture(diffuseTexture, texCoord) * attenIntensity * cosAngIncidence) +
    // (specularColor * attenIntensity * gaussianTerm);        
}

vec4 gammaCorrect(in vec4 color)
{
    vec4 gammaInv = vec4(Lgt.gammaInv);
    gammaInv.w = 1.0;

    return(pow(color, gammaInv));
}

void main()
{
    // vec4 accumLighting = texture(diffuseTexture, texCoord) * Lgt.ambientIntensity;
    vec4 accumLighting = color * Lgt.ambientIntensity;

    vec3 surfaceNormal = normalize(vertexNormalCamSpace);
    vec3 viewDir = normalize(-cameraSpacePosition);

    for (int i = 0; i < numberOfLights; ++i)
    {
        // Only compute lights that are on
        if (Lgt.lights[i].isOn == true)
        {
            accumLighting += calcLighting(Lgt.lights[i]);
        }
    }

    // HDR
    accumLighting = accumLighting / Lgt.maxIntensity;

    accumLighting = gammaCorrect(accumLighting);
    
    outputColor = accumLighting;
}
