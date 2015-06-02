#version 330

in vec3 vPos;
in vec3 vNorm;
in vec2 vTex;

// Model-to-world matrix
uniform mat4 mvpMat;

out float depth;

void main() {
	gl_Position = mvpMat * vec4(vPos, 1.0);
	gl_ClipDistance[0] = dot(vec4(0,0,-1,0), gl_Position);
	depth = gl_Position.w;
}