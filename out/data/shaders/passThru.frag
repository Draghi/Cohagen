#version 410

in float depth;

out vec4 fragColour;

void main() {
	fragColour = vec4(1.0, depth/10, depth/10, 1.0);
}
