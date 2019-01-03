#include "FrustumCullingShaderProgram.h"

FrustumCullingShaderProgram::FrustumCullingShaderProgram()
{
	createShaderData();
	createSSBO();
}

FrustumCullingShaderProgram::~FrustumCullingShaderProgram()
{
}

void FrustumCullingShaderProgram::createShaderData()
{
	ShaderProgram temp = ShaderProgram();
	gShaderProgram = temp.CreateShaderData("", "", "", "", "", "FrustumCullingCS.glsl");
}

void FrustumCullingShaderProgram::createSSBO()
{
	glGenBuffers(1, &oobbStorageBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, oobbStorageBuffer);

	glGenBuffers(1, &positionStorageBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionStorageBuffer);

	glGenBuffers(1, &indiciesStorageBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, indiciesStorageBuffer);

	glGenBuffers(1, &frustumPlaneStorageBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, frustumPlaneStorageBuffer);

	
}

std::vector<GameObject*> FrustumCullingShaderProgram::activateShaderProgram(std::vector<GameObject*> otherGameObjects, GameObject* camera,unsigned int display_w, unsigned int display_h)
{
	//std::cout << "Frustum!" << otherGameObjects.size()<< std::endl;

	std::vector<OOBB> OOBBs = std::vector<OOBB>();
	std::vector<glm::vec4> position = std::vector<glm::vec4>();
	std::vector<int> indicies = std::vector<int>();
	std::vector<PLANE> frustumPlane = std::vector<PLANE>();

	Camera* mainCamera = camera->getComponent<Camera>();
	glm::mat4x4 projection_matrix = mainCamera->calculatePerspectiveMatrix(display_w, display_h);
	glm::mat4x4 view_matrix = mainCamera->calculateViewMatrix();
	glm::mat4 PV = projection_matrix * view_matrix*glm::mat4(1);
	PV = glm::transpose(PV);

	float denom = 1;
	PLANE leftP;
	for (int i = 0; i < 4; i++)
	{
		leftP.xyz_d[i] = -(PV[3][i]+ PV[0][i]);
	}
	denom = 1 / length(glm::vec3(leftP.xyz_d));
	leftP.xyz_d *= denom;
	frustumPlane.push_back(leftP);

	PLANE rightP;
	for (int i = 0; i < 4; i++)
	{
		rightP.xyz_d[i] = -(PV[3][i] - PV[0][i]);
	}
	denom = 1 / length(glm::vec3(rightP.xyz_d));
	rightP.xyz_d *= denom;
	frustumPlane.push_back(rightP);

	PLANE topP;
	for (int i = 0; i < 4; i++)
	{
		topP.xyz_d[i] = -(PV[3][i] - PV[1][i]);
	}
	denom = 1 / length(glm::vec3(topP.xyz_d));
	topP.xyz_d *= denom;
	frustumPlane.push_back(topP);

	PLANE bottomP;
	for (int i = 0; i < 4; i++)
	{
		bottomP.xyz_d[i] = -(PV[3][i] + PV[1][i]);
	}
	denom = 1 / length(glm::vec3(bottomP.xyz_d));
	bottomP.xyz_d *= denom;
	frustumPlane.push_back(bottomP);

	PLANE farP;
	for (int i = 0; i < 4; i++)
	{
		farP.xyz_d[i] = -(PV[3][i] - PV[2][i]);
	}
	denom = 1 / length(glm::vec3(farP.xyz_d));
	farP.xyz_d *= denom;
	frustumPlane.push_back(farP);

	PLANE nearP;
	for (int i = 0; i < 4; i++)
	{
		nearP.xyz_d[i] = -(PV[3][i] + PV[2][i]);
	}
	denom = 1 / length(glm::vec3(nearP.xyz_d));
	nearP.xyz_d *= denom;
	frustumPlane.push_back(nearP);


	for (int i = 0; i < otherGameObjects.size(); i++)
	{
		OOBBs.push_back(otherGameObjects[i]->OOBoundingBox);
		position.push_back(glm::vec4(otherGameObjects[i]->transform.position,1));
		indicies.push_back(i);
	}

	glUseProgram(gShaderProgram);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, oobbStorageBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, otherGameObjects.size() * sizeof(OOBB), OOBBs.data(), GL_DYNAMIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, oobbStorageBuffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionStorageBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, otherGameObjects.size() * sizeof(glm::vec4), position.data(), GL_DYNAMIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, positionStorageBuffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, indiciesStorageBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, otherGameObjects.size() * sizeof(int), indicies.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, indiciesStorageBuffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, frustumPlaneStorageBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, otherGameObjects.size() * sizeof(PLANE), frustumPlane.data(), GL_DYNAMIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, frustumPlaneStorageBuffer);


	glDispatchCompute(otherGameObjects.size(), 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, indiciesStorageBuffer);
	int* ptr = (int*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	//system("cls");

	glm::vec3 corners[8];
	corners[0] = OOBBs[4].x_hx.w*glm::vec3(OOBBs[4].x_hx) + OOBBs[4].y_hy.w*glm::vec3(OOBBs[4].y_hy) + OOBBs[4].z_hz.w*glm::vec3(OOBBs[4].z_hz) + glm::vec3(OOBBs[4].centre) + glm::vec3(position[4]);
	corners[1] = -OOBBs[4].x_hx.w*glm::vec3(OOBBs[4].x_hx) - OOBBs[4].y_hy.w*glm::vec3(OOBBs[4].y_hy) - OOBBs[4].z_hz.w*glm::vec3(OOBBs[4].z_hz) + glm::vec3(OOBBs[4].centre) + glm::vec3(position[4]);

	corners[2] = OOBBs[4].x_hx.w*glm::vec3(OOBBs[4].x_hx) + OOBBs[4].y_hy.w*glm::vec3(OOBBs[4].y_hy) - OOBBs[4].z_hz.w*glm::vec3(OOBBs[4].z_hz) + glm::vec3(OOBBs[4].centre) + glm::vec3(position[4]);
	corners[3] = -OOBBs[4].x_hx.w*glm::vec3(OOBBs[4].x_hx) - OOBBs[4].y_hy.w*glm::vec3(OOBBs[4].y_hy) + OOBBs[4].z_hz.w*glm::vec3(OOBBs[4].z_hz) + glm::vec3(OOBBs[4].centre) + glm::vec3(position[4]);

	corners[4] = -OOBBs[4].x_hx.w*glm::vec3(OOBBs[4].x_hx) + OOBBs[4].y_hy.w*glm::vec3(OOBBs[4].y_hy) + OOBBs[4].z_hz.w*glm::vec3(OOBBs[4].z_hz) + glm::vec3(OOBBs[4].centre) + glm::vec3(position[4]);
	corners[5] = OOBBs[4].x_hx.w*glm::vec3(OOBBs[4].x_hx) - OOBBs[4].y_hy.w*glm::vec3(OOBBs[4].y_hy) - OOBBs[4].z_hz.w*glm::vec3(OOBBs[4].z_hz) + glm::vec3(OOBBs[4].centre) + glm::vec3(position[4]);

	corners[6] = -OOBBs[4].x_hx.w*glm::vec3(OOBBs[4].x_hx) + OOBBs[4].y_hy.w*glm::vec3(OOBBs[4].y_hy) - OOBBs[4].z_hz.w*glm::vec3(OOBBs[4].z_hz) + glm::vec3(OOBBs[4].centre) + glm::vec3(position[4]);
	corners[7] = OOBBs[4].x_hx.w*glm::vec3(OOBBs[4].x_hx) - OOBBs[4].y_hy.w*glm::vec3(OOBBs[4].y_hy) + OOBBs[4].z_hz.w*glm::vec3(OOBBs[4].z_hz) + glm::vec3(OOBBs[4].centre) + glm::vec3(position[4]);
	//for (int i = 0; i < frustumPlane.size(); i++)
	//{
	//	std::cout << "OBJ BBOX " << corners[i].x << " , " << corners[i].y << " , " << corners[i].z << std::endl;

	//}


	std::vector<GameObject*> collidingGO;
	for (int i = 0; i < otherGameObjects.size(); i++)
	{
		//std::cout << "Frustum Indicies " << ptr[i] << std::endl;
		if (ptr[i]>0 && otherGameObjects[i]->getIsRenderable() == true) {
			collidingGO.push_back(otherGameObjects[i]);

		}
	}

	//for (int i = 0; i < frustumPlane.size(); i++)
	//{
	//	std::cout << "Frustum Plane Normals " <<frustumPlane[i].xyz_d.x<<" , " << frustumPlane[i].xyz_d.y << " , " << frustumPlane[i].xyz_d.z << " D:" << frustumPlane[i].xyz_d.w << " " << std::endl;

	//}


	return collidingGO;
}
