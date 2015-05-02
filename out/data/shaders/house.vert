#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform sampler2D tex;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec2 tc;

void main() {
	tc = texCoord.xy;
	gl_Position = projectionMatrix * modelMatrix * vec4(position.x, position.y, position.z, 1.0);
}