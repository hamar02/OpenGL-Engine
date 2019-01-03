#include "Camera.h"
#include "GameObject.h"

Camera::Camera() {
}

Camera::Camera(GLuint gShaderProgram)
{
	assetName = "Camera";
    this->gShaderProgram = gShaderProgram;

}

Camera::~Camera()
{
}

void Camera::update()
{
	//glm::mat4x4 view_matrix = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 1.0f, 0.0f));
	//gameObject->transform.position = glm::vec3(-2.0f, 4.0f, -1.0f);

	//std::cout << "cam Matrix, uniformlocation ="<< uniformLoc<< std::endl << view_matrix[0][0] << view_matrix[1][0] << view_matrix[2][0] << std::endl << view_matrix[0][1] << view_matrix[1][1] << view_matrix[2][1] <<std::endl<< view_matrix[0][2] << view_matrix[1][2] << view_matrix[2][2] << std::endl;

}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(gameObject->transform.position, gameObject->transform.position + gameObject->transform.forward, gameObject->transform.up);
}

glm::mat4 Camera::calculatePerspectiveMatrix(unsigned int display_w, unsigned int display_h)
{
	return glm::perspective((float)(3.14*0.45), float(display_w) / float(display_h), 0.1f, 100.0f);
}
