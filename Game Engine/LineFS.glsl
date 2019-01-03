#version 430

out vec4 fragment_color;

in VS_OUT{
	vec2 TexCoords;
} vs_out;


void main () {
	
	fragment_color = vec4(1,0,0, 1);

}