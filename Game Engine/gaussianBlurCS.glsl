#version 430
#define EPSILON 0.0001

layout ( binding = 0, rgba32f) uniform image2D inImage;
layout ( binding = 1, rgba32f) uniform image2D outImage;
uniform int horizontal;
uniform int colorChannels;
layout(local_size_x = 32, local_size_y = 30) in; //gl_GlobalInvocationID.xy(z?) gl_LocalInvocationID.xy(z?)
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main(){
	//RED
	if(colorChannels == 1){
		ivec2 texture_offset = ivec2(1,1);
		float result = imageLoad(inImage, ivec2(gl_GlobalInvocationID.xy)).r*weight[0];

		if (horizontal==1) {
			for (int i = 1; i < 5; ++i)
			{
				result += imageLoad(inImage, ivec2(gl_GlobalInvocationID.xy) + ivec2(texture_offset.x*i, 0.0)).r*weight[i];
				result += imageLoad(inImage, ivec2(gl_GlobalInvocationID.xy) - ivec2(texture_offset.x*i, 0.0)).r*weight[i];
			}
		}
		else {
			for (int i = 1; i < 5; ++i)
			{
				result += imageLoad(inImage, ivec2(gl_GlobalInvocationID.xy) + ivec2(0.0,texture_offset.y*i)).r*weight[i];
				result += imageLoad(inImage, ivec2(gl_GlobalInvocationID.xy) - ivec2(0.0,texture_offset.y*i)).r*weight[i];
			}
		}
		imageStore(outImage, ivec2(gl_GlobalInvocationID.xy), vec4(result.r, result.r, result.r,1.0));
	
	}

	//RGB
	if(colorChannels == 3){
		ivec2 texture_offset = ivec2(1,1);
		vec3 result = imageLoad(inImage, ivec2(gl_GlobalInvocationID.xy)).rgb*weight[0];

		if (horizontal==1) {
			for (int i = 1; i < 5; ++i)
			{
				result += imageLoad(inImage, ivec2(gl_GlobalInvocationID.xy) + ivec2(texture_offset.x*i, 0.0)).rgb*weight[i];
				result += imageLoad(inImage, ivec2(gl_GlobalInvocationID.xy) - ivec2(texture_offset.x*i, 0.0)).rgb*weight[i];
			}
		}
		else {
			for (int i = 1; i < 5; ++i)
			{
				result += imageLoad(inImage, ivec2(gl_GlobalInvocationID.xy) + ivec2(0.0,texture_offset.y*i)).rgb*weight[i];
				result += imageLoad(inImage, ivec2(gl_GlobalInvocationID.xy) - ivec2(0.0,texture_offset.y*i)).rgb*weight[i];
			}
		}
		imageStore(outImage, ivec2(gl_GlobalInvocationID.xy), vec4(result.xyz,1.0));
	
	}
}