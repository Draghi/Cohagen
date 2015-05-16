// Taken from: http://gamedev.stackexchange.com/questions/60313/implementing-a-skybox-with-glsl-version-330

#version 330

layout (location = 0) in vec4 position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

smooth out vec3 eyeDir;

void main() {
    mat4 invCamToClip = inverse(projectionMatrix);
    mat3 invModelView = transpose(mat3(viewMatrix));
    vec3 unprojected = (invCamToClip * position).xyz;
    eyeDir = (invModelView * unprojected);

    eyeDir.x = eyeDir.x;
    eyeDir.y = -eyeDir.y;
    eyeDir.z = -eyeDir.z;

    gl_Position = position;
}