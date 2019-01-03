#pragma once
#include <GL/gl3w.h>  
#include"Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera :public Component
{
public:
    Camera();
	Camera(GLuint gShaderProgram);
	~Camera();

    GLuint gShaderProgram;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
	void update();
	glm::mat4 calculateViewMatrix();
	glm::mat4 calculatePerspectiveMatrix(unsigned int display_w, unsigned int display_h);
	
private:

};

