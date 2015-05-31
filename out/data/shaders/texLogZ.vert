#version 330

in vec3 vPos;
in vec3 vNorm;
in vec2 vTex;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform mat4 mvpMat;

uniform float FCoef;

out vec2 texCoord;
out float logz;

const float depthRes = 0.01;

void main() {
	texCoord = vTex;
	
	gl_Position = mvpMat * vec4(vPos, 1.0);	
	
	//Log depth calculations from:
	//http://outerra.blogspot.com.au/2012/11/maximizing-depth-buffer-range-and.html
	logz = log2(max(1e-6, gl_Position.w*depthRes + 1))*FCoef;
    gl_Position.z = (2*logz - 1)*gl_Position.w;
}
