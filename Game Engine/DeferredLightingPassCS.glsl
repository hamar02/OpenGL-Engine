#version 430
#define EPSILON 0.0001
const float PI = 3.14159265359;

layout(local_size_x = 32, local_size_y = 30) in; //gl_GlobalInvocationID.xy(z?) gl_LocalInvocationID.xy(z?)

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedo;
layout(binding = 3) uniform sampler2D gAORoughMet;
layout(binding = 4) uniform sampler2D gEmissive;
layout(binding = 5) uniform sampler2D gSSAO;

layout ( binding = 6, rgba32f) uniform image2D FragColor;
layout ( binding = 7, rgba32f) uniform image2D FragHDRColor;

uniform vec3 view_position;
uniform mat4 invView_matrix;
uniform mat4 view_matrix;

const int NR_LIGHTS = 26;
struct Light {
	int lightType;
    vec3 Position;
	vec3 DirectionVector;
    vec3 Color;
	float Intensity;
	float Range;
	mat4 lightSpace_matrix;
	sampler2D shadowMap;

};
uniform Light lights[NR_LIGHTS];




float distributionGGX(vec3 normalV, vec3 halfwayV, float roughness){
    
    //Based on observations by Disney and adopted by Epic Games the 
    //lighting looks more correct squaring the roughness in both 
    //the geometry and normal distribution function.
    float roughness2 = roughness*roughness*roughness*roughness;
    float normalVdotHalfwayV=max(dot(normalV, halfwayV), 0.0);
    float normalVdotHalfwayV2=normalVdotHalfwayV*normalVdotHalfwayV;

    float denominator = normalVdotHalfwayV2*(roughness2-1.0)+1.0;
    denominator= PI *denominator*denominator;

    return roughness2/denominator;
}

float geometrySchlickGGX(float normalVdotViewDirV, float roughness){

    float rough = roughness+1.0;
    float k = (rough*rough)/8.0;
    float denominator = normalVdotViewDirV*(1.0-k)+k;

    return normalVdotViewDirV/denominator;
}

float GeometrySmith(vec3 normalV, vec3 viewDirV, vec3 lightDirV, float k){

    float normalVdotViewDirV = max(dot(normalV, viewDirV), 0.0);
    float normalVdotLightDirV = max(dot(normalV, lightDirV), 0.0);
    float ggx1 = geometrySchlickGGX(normalVdotViewDirV, k);
    float ggx2 = geometrySchlickGGX(normalVdotLightDirV, k);

    return ggx1*ggx2;
}

vec3 fresnelSchlik(float cosTheta, vec3 fresnel0){
    return fresnel0+(1.0-fresnel0)*pow(1.0 -cosTheta, 5.0);
}

void main(){
//	// retrieve data from G-buffer
	vec2 uvCoord = vec2(gl_GlobalInvocationID.xy)/vec2(1280,720);// screen = 1280x720
	vec3 FragPos = texture(gPosition, uvCoord).rgb;
	vec3 Normal = texture(gNormal, uvCoord).rgb;
	vec3 Albedo = texture(gAlbedo, uvCoord).rgb;
	vec3 AORoughMet = texture(gAORoughMet, uvCoord).rgb;
	vec3 Emissive = texture(gEmissive, uvCoord).rgb;
	float SSAO = texture(gSSAO, uvCoord).r;
	vec3 viewDir = normalize((view_matrix*vec4(view_position, 1)).xyz - FragPos);

    //Base reflectivity
    vec3 fresnel0 = vec3(0.04);
    fresnel0 = mix(fresnel0, Albedo, AORoughMet.b);

    vec3 totalSpectralRadiance = vec3(0.0);


	//Lights
	for (int i = 0; i < NR_LIGHTS; ++i)
	{
		vec3 lightDir;
		vec3 halfwayDir;
		vec3 radiance;
		float shadow=0;
		if(lights[i].lightType==0){
		//Spotlight
		}

		//Directional
		if(lights[i].lightType==1){
			lightDir = normalize(lights[i].DirectionVector);
			lightDir = (view_matrix*vec4(lightDir, 0)).xyz;

			halfwayDir = normalize(lightDir + viewDir);

			//Shadow Calculations
			vec4 fragPosLightSpace = lights[i].lightSpace_matrix*invView_matrix*vec4(FragPos, 1.0);
			vec3 projectedCoordninates = fragPosLightSpace.xyz / fragPosLightSpace.w;
			projectedCoordninates = projectedCoordninates*0.5 + 0.5;
			float currentDepth = projectedCoordninates.z;
			vec2 texelSize = 1.0 / textureSize(lights[0].shadowMap, 0);
			if (currentDepth < 1.0) {
				float bias = max(0.05 * (dot(Normal, lightDir)), 0.05);

				for (int x = -1; x <= 1; ++x)
				{
					for (int y = -1; y <= 1; ++y)
					{
						float pcfDepth = texture(lights[0].shadowMap, projectedCoordninates.xy + vec2(x, y) * texelSize).r;
						shadow += currentDepth-bias > pcfDepth ? 1.0 : 0.0;
					}
				}
			}
			shadow = shadow / 9.0;
			//End Shadow Calculations

			radiance = lights[i].Color*lights[i].Intensity;
		}
		
		//Point
		if(lights[i].lightType==2){
			lightDir = normalize((view_matrix*vec4( lights[i].Position, 1)).xyz  - FragPos);
			halfwayDir = normalize(lightDir + viewDir);  
			//
			//Shadow
			//
			float distance = length((view_matrix*vec4( lights[i].Position, 1)).xyz - FragPos);
			float attenuation = 1.0 / (distance*distance); 
			radiance = lights[i].Color*lights[i].Intensity*attenuation;

		}




		//Fresnel
		vec3 fresnel = fresnelSchlik(clamp(dot(halfwayDir, viewDir), 0.0, 1.0), fresnel0);

		//Normal distribution function: approximates the amount the surface's microfacets are aligned to the halfway vector influenced by the roughness of the surface
		float normalDistribution = distributionGGX(Normal, halfwayDir, AORoughMet.g);

		//Geometry function: describes the self-shadowing property of the microfacets.
		float geometryValue = GeometrySmith(Normal, viewDir, lightDir, AORoughMet.g);

		//Cook-Torrance bidirectional reflectance distribution function 
		vec3 nominator = normalDistribution*geometryValue*fresnel;
		float denominator = 4.0 * max(dot(Normal, viewDir), 0.0)*max(dot(Normal, lightDir), 0.0);
		vec3 specular = nominator / max(denominator, 0.001);

		vec3 contributionSpec = fresnel;
		vec3 contributionDif = vec3(1.0) - contributionSpec;
		contributionDif *= 1.0 - AORoughMet.b;

		float normalVdotLightDirV = max(dot(Normal, lightDir), 0.0);
		totalSpectralRadiance += (contributionDif*Albedo / PI + specular)*radiance*normalVdotLightDirV*(1-shadow);
	}
	float ao = AORoughMet.r;
	if (ao < SSAO) {
		ao = SSAO;
	}


    vec3 ambient = vec3(0.03)*Albedo;
    vec3 color= ambient + totalSpectralRadiance*ao;

	//HDR Output
	vec3 HDRValue = vec3(0, 0, 0);
	float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0) {
		HDRValue += color;
	}
	HDRValue += Emissive;
    imageStore(FragHDRColor, ivec2(gl_GlobalInvocationID.xy), vec4(HDRValue.xyz,1.0));

	vec3 outputColor = vec3(0, 0, 0);
	brightness = dot(color.rgb, vec3(1.0,1.0,1.0));
	if (brightness > EPSILON) {
		outputColor += color;
	}
    imageStore(FragColor, ivec2(gl_GlobalInvocationID.xy), vec4(outputColor.xyz,1.0));

}