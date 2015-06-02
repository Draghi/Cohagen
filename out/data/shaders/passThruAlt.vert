#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

// Model-to-world matrix
uniform mat4 mvpMat;

out float depth;

void main() {
	gl_Position = mvpMat * vec4(position, 1.0);
	depth = gl_Position.w;
}