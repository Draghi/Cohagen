#version 330

in vec2 tc;

uniform sampler2D tex;

out vec4 fragColour;

void main() {
	fragColour = texture(tex, tc);
}
