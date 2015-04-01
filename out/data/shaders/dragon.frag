#version 330

in float z;

out vec4 fragColour;

void main() {
	fragColour = vec4(z+0.5, z, z-0.5, 1.0);
}
