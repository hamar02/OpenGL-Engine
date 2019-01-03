#version 430
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

int kernelSize = 64;
float radius = 0.15;
float bias = 0.005;
const vec2 noiseScale = vec2(1280.0 / 4.0, 720.0 / 4.0); // screen = 1280x720


uniform mat4 projection;

void main()
{
	// get input for SSAO
	vec3 fragPos = texture(gPosition, TexCoords).xyz;
	vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
	vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
	
	// create TBN
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	// iterate over the sample kernel
	float occlusion = 0.0;
	for (int i = 0; i < kernelSize; ++i)
	{
		// get view space sample
		vec3 sampleWS = TBN * samples[i]; // from tangent to view-space
		sampleWS = fragPos + sampleWS * radius;
		
		// project sample position to ndc to sample depth
		vec4 offset = vec4(sampleWS, 1.0);
		offset = projection * offset; // from view to clip-space
		offset.xyz /= offset.w; // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

											 // get sample depth
		float sampleDepth = texture(gPosition, offset.xy).z; // get depth value of kernel sample

															 // range check & accumulate
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= sampleWS.z + bias ? 1.0 : 0.0) * rangeCheck;
	}
	//calculate the average occlusion factor
	occlusion = 1.0 - (occlusion / kernelSize);

	FragColor = occlusion;
}