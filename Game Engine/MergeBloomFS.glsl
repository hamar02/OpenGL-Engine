#version 430

in vec2 uvCoordVS;
uniform sampler2D textureColorIn;
uniform sampler2D textureBloomIn;
uniform float exposure;

out vec4 fragment_color;

void main() {
	const float gamma = 2.2;
	vec3 color = texture(textureColorIn, uvCoordVS).rgb;
	vec3 bloom= texture(textureBloomIn, uvCoordVS).rgb;

	color+= bloom;
	vec3 result = vec3(1.0) - exp(-color * exposure);
	
	// gamma correct    
	result = pow(result, vec3(1.0 / gamma));


	fragment_color = vec4(result,1.0);

}