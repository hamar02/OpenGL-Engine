#include "Transform.h"

Transform::Transform(): Component()
{
	assetName = "Transform";

	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

Transform::Transform(glm::vec3 otherPosition)
{
	assetName = "Transform";
	position = otherPosition;
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

Transform::~Transform()
{
}

void Transform::update()
{
}

void Transform::updateRotationInEuler()
{
	glm::mat4 rotationMatrix=glm::mat4(1);
	//rot X
	rotationMatrix=glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	//rot Y
	rotationMatrix=glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	//rot Z
	rotationMatrix=glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 forwardT = rotationMatrix*glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec4 upT= rotationMatrix*glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec4 rightT= rotationMatrix*glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	forward = glm::vec3(forwardT.x, forwardT.y, forwardT.z);
	up= glm::vec3(upT.x, upT.y, upT.z);
	right= glm::vec3(rightT.x, rightT.y, rightT.z);
	//std::cout << "Rot Vector update" << std::endl << rotation.x << rotation.y << rotation.z << std::endl;
	//std::cout << "up Vector update" <<std::endl<< rotationMatrix[0][0] << rotationMatrix[1][0] << rotationMatrix[2][0] << rotationMatrix[3][0] << std::endl << rotationMatrix[0][1] << rotationMatrix[1][1] << rotationMatrix[2][1] << rotationMatrix[3][1] << std::endl << rotationMatrix[0][2] << rotationMatrix[1][2] << rotationMatrix[2][2] << rotationMatrix[3][2] << std::endl << rotationMatrix[0][3] << rotationMatrix[1][3] << rotationMatrix[2][3] << rotationMatrix[3][3] << std::endl;

}
