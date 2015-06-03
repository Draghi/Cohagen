#version 330

in vec3 vPos;
in vec3 vNorm;
in vec2 vTex;

uniform mat4 mvpMat;

out vec2 tc;

void main() {
	gl_Position = mvpMat*vec4(vPos, 1.0);
	tc = vTex;
}