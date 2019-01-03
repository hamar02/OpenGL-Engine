#version 430

struct obb_type {
	vec4 centre;
	vec4 x_hx;
	vec4 y_hy;
	vec4 z_hz;
};

struct plane {
	vec3 xyzN;
	float d;
};

layout(binding = 0) buffer oob
{
	obb_type obb_data[];
};

layout(binding = 1) buffer positions
{
	vec4 positionsGameObject[];
};

layout(binding = 2) writeonly buffer indicies
{
	int indiciesGameObject[];
};

layout(binding = 3) buffer frustumPlanes
{
	plane frustumPlane[];
};


layout(local_size_x = 1, local_size_y = 1) in; //gl_GlobalInvocationID.xy(z?) gl_LocalInvocationID.xy(z?)

int planeAABBIntersect(obb_type otherOBB, vec3 otherPosition, plane frustumPlane) {
	vec3 corners[8];
	corners[0] = otherOBB.x_hx.w*otherOBB.x_hx.xyz + otherOBB.y_hy.w*otherOBB.y_hy.xyz + otherOBB.z_hz.w*otherOBB.z_hz.xyz+ otherOBB.centre.xyz+ otherPosition;
	corners[1] = -otherOBB.x_hx.w*otherOBB.x_hx.xyz - otherOBB.y_hy.w*otherOBB.y_hy.xyz - otherOBB.z_hz.w*otherOBB.z_hz.xyz + otherOBB.centre.xyz + otherPosition;
	
	corners[2] = otherOBB.x_hx.w*otherOBB.x_hx.xyz + otherOBB.y_hy.w*otherOBB.y_hy.xyz - otherOBB.z_hz.w*otherOBB.z_hz.xyz + otherOBB.centre.xyz + otherPosition;
	corners[3] = -otherOBB.x_hx.w*otherOBB.x_hx.xyz - otherOBB.y_hy.w*otherOBB.y_hy.xyz + otherOBB.z_hz.w*otherOBB.z_hz.xyz + otherOBB.centre.xyz + otherPosition;
	
	corners[4] = -otherOBB.x_hx.w*otherOBB.x_hx.xyz + otherOBB.y_hy.w*otherOBB.y_hy.xyz + otherOBB.z_hz.w*otherOBB.z_hz.xyz + otherOBB.centre.xyz + otherPosition;
	corners[5] = otherOBB.x_hx.w*otherOBB.x_hx.xyz - otherOBB.y_hy.w*otherOBB.y_hy.xyz - otherOBB.z_hz.w*otherOBB.z_hz.xyz + otherOBB.centre.xyz + otherPosition;
	
	corners[6] = -otherOBB.x_hx.w*otherOBB.x_hx.xyz+ otherOBB.y_hy.w*otherOBB.y_hy.xyz - otherOBB.z_hz.w*otherOBB.z_hz.xyz + otherOBB.centre.xyz + otherPosition;
	corners[7] = otherOBB.x_hx.w*otherOBB.x_hx.xyz - otherOBB.y_hy.w*otherOBB.y_hy.xyz + otherOBB.z_hz.w*otherOBB.z_hz.xyz + otherOBB.centre.xyz + otherPosition;



	vec3 diagonals[4];
	diagonals[0] = corners[0] - corners[1];
	diagonals[1] = corners[2] - corners[3];
	diagonals[2] = corners[4] - corners[5];
	diagonals[3] = corners[6] - corners[7];

	//

	float lengthV = 0;
	for (int i = 0; i < 4; i++) {
		float len = abs(dot(diagonals[i], frustumPlane.xyzN));
		if (len > lengthV) {
			lengthV = len;
		}
	}

	float e = lengthV / 2;
	float s = dot((otherOBB.centre.xyz + otherPosition), frustumPlane.xyzN) + frustumPlane.d;//+d plane equation

	if ((s - e) > 0) {
		return 0;
	}
	if ((s + e) < 0) {
		return 1;
	}
	return 2;
}

int frustumCheck(obb_type otherOBB, vec3 otherPosition){
	int intersecting =0;
	for(int i=0; i<6;i++){
		int result = planeAABBIntersect(otherOBB, otherPosition, frustumPlane[i]);
		if(result == 0){
			return 0;
		}
		else if(result == 2){
			intersecting = 1;
		}
		
	}
	if(intersecting == 1){
		return 2;
	}else{
		return 1;
	}

}

void main() {
	obb_type obb = obb_data[gl_GlobalInvocationID.x];
	vec3 obbPosition = positionsGameObject[gl_GlobalInvocationID.x].xyz;
	
		

	//0 outside
	//1 inside
	//2 intersecting
	indiciesGameObject[gl_GlobalInvocationID.x] = frustumCheck(obb, obbPosition); //result;//-1;

		

}