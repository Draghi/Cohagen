#version 330
#extension GL_ARB_conservative_depth : enable

in vec2 texCoord;
in float logz;

uniform sampler2D tex;

out vec4 fragColour;
layout(depth_less) out float gl_FragDepth;

void main() {
	fragColour = texture(tex, texCoord);
	gl_FragDepth = logz;
}
