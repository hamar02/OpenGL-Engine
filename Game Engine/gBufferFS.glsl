#version 430
layout(location = 0) out vec4 gPositionRough;
layout(location = 1) out vec4 gNormalAO;
layout(location = 2) out vec4 gAlbedo;
layout(location = 3) out vec4 gEmissiveMet;


in VS_OUT {
    vec4 FragPos;
    vec2 TexCoords;
    mat3 viewTBN;
} fs_in;

uniform sampler2D gAlbedoIn;
uniform sampler2D gNormalIn;
uniform sampler2D gAORoughMetIn;
uniform sampler2D gEmissiveIn;

//importera textures med flipped uv!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void main () {
	vec3 gAORoughMet = texture(gAORoughMetIn, vec2(fs_in.TexCoords.s,  fs_in.TexCoords.t)).xyz;

	gPositionRough = vec4(fs_in.FragPos.xyz, gAORoughMet.y);

	vec3 norm = texture(gNormalIn, vec2(fs_in.TexCoords.s, fs_in.TexCoords.t)).rgb;
	norm = norm * 2.0 - 1.0;
	gNormalAO = vec4(normalize(fs_in.viewTBN*norm), gAORoughMet.x);
	  

	gAlbedo =  vec4(texture(gAlbedoIn, vec2(fs_in.TexCoords.s, fs_in.TexCoords.t)).xyz,1);
	gEmissiveMet = vec4(texture(gEmissiveIn, vec2(fs_in.TexCoords.s, fs_in.TexCoords.t)).xyz, gAORoughMet.z);


}