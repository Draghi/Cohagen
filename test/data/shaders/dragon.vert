#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out float z;

void main() {
	z = position.y;
	gl_Position = vec4(position.x, position.z, position.y, 1.0);
}