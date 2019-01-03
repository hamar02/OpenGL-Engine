#pragma once
#include <GL/gl3w.h>  
#include"Component.h"
#include<iostream>
#include <Windows.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CharacterMovement :public Component
{
public:
	CharacterMovement();
	CharacterMovement(GLFWwindow* window);
	~CharacterMovement();

	GLFWwindow* window;

	void update();
private:

	float cameraSpeed;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float pitch;
	float yaw;
	bool firstMouse;
	float lastX, lastY;
	double xpos, ypos;
	int mouseDisable;
    float xoffset;
    float yoffset;
    float sensitivity;

};

