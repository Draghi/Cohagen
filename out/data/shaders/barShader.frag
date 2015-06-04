#version 330

in vec2 tc;

uniform sampler2D tex;

out vec4 fragColour;

void main() {
	fragColour = vec4(1.0f, 0.0f, 0.0f, 1.0f);//texture(tex, tc);
}
