#version 410

in float depth;

out vec4 fragColour;

void main() {
	float r = abs(mod(depth, 2)/1-1);
	float g = abs(mod(depth, 4)/2-1);
	float b = abs(mod(depth, 8)/4-1);
	
	fragColour = vec4(r, g, b, 1.0);
}
