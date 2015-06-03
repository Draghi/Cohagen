#version 330

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTex;

out vec2 tc;

void main() {
	gl_Position = vec4(vPos, 1.0);
	tc = vTex;
}