#version 430

in vec2 uvCoordVS;
uniform sampler2D textureColorIn;

out vec4 fragment_color;

void main() {
	vec3 color = texture(textureColorIn, uvCoordVS).rgb;
	fragment_color = vec4(color, 1.0);

}