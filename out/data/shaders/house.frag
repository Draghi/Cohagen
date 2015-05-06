#version 410

in vec2 tc;

uniform sampler2D tex;

out vec4 fragColour;

void main() {
	fragColour = texture2D(tex, tc);
}
