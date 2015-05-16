// http://gamedev.stackexchange.com/questions/60313/implementing-a-skybox-with-glsl-version-330

#version 330

uniform samplerCube cubeTexture;

smooth in vec3 eyeDir;

out vec4 fragmentColor;

void main() {
    fragmentColor = texture(cubeTexture, eyeDir);
}