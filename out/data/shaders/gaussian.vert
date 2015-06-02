#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 cameraSpacePosition;
out vec3 vertexNormalCamSpace;
out vec2 tc;
out vec4 color;

uniform vec4 colorIn;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    gl_Position = projectionMatrix * (viewMatrix * (modelMatrix * vec4(position, 1.0f)));

    cameraSpacePosition = vec3(viewMatrix * (modelMatrix * vec4(position, 1.0f)));
    vertexNormalCamSpace = vec3(viewMatrix * modelMatrix * vec4(normal, 0.0));

    tc = texCoord;
    color = colorIn;
}
