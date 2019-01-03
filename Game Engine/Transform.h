#pragma once
#include <GL/gl3w.h>  
#include"Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Transform :public Component
{
public:
	Transform();
	Transform(glm::vec3 otherPosition);
	~Transform();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

    glm::vec3 forward = glm::vec3(1, 0, 0);
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 right = glm::vec3(0, 0, 1);

    
	void update();
	void updateRotationInEuler();
private:

};

