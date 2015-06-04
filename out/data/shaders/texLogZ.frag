#version 330
//#extension GL_ARB_conservative_depth : enable

in vec2 texCoord;
in float logz;
in vec3 pos;

uniform sampler2D tex;

out vec4 fragColour;
//layout(depth_less) out float gl_FragDepth;

void main() {
	float dist = max(0.2, (300-length(pos))/300);
	fragColour = texture(tex, texCoord)*dist;
	gl_FragDepth = logz;
}
