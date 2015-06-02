#version 330

in vec3 vPos;
in vec3 vNorm;
in vec2 vTex;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform mat4 mvpMat;

uniform float near;
uniform float FCoef;

out vec2 texCoord;
out float logz;

void main() {
	texCoord = vTex;
	
	gl_Position = mvpMat * vec4(vPos, 1.0);	
	gl_ClipDistance[0] = dot(vec4(0,0,-1,0), gl_Position);
	
	//Log depth calculations from:
	//http://outerra.blogspot.com.au/2012/11/maximizing-depth-buffer-range-and.html
	logz = log(max(0.5397606e-78, gl_Position.w*near + 1))*FCoef;
    gl_Position.z = (2*logz - 1)*gl_Position.w;
}
