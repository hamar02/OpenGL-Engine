#include "CharacterMovement.h"
#include "GameObject.h"

CharacterMovement::CharacterMovement()
{
}

CharacterMovement::CharacterMovement(GLFWwindow * window)
{
    assetName = "CharacterMovement";
	this->window = window;
	cameraSpeed = 0.03f;
	cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	pitch = 0;
	yaw = 0;
	firstMouse = true;
	lastX = 400;
	lastY = 300;
	xpos = 0;
	ypos = 0;
    mouseDisable = 0;
    xoffset = 0;
    yoffset = 0;
    sensitivity = 0.002f;
}

CharacterMovement::~CharacterMovement()
{
}

void CharacterMovement::update()
{
	
	//... Petter - Mouse Movement
	glfwGetCursorPos(window, &xpos, &ypos);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
	//std::cout << "update" << std::endl;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw = xoffset;
        pitch = yoffset;

        if (pitch > 90.f)
            pitch = 90.0f;
        if (pitch < -90.0f)
            pitch = -90.0f;

        glm::mat4 matrix = glm::mat4(1);

        glm::vec4 forward = glm::vec4(gameObject->transform.forward, 0);
        glm::vec4 right = glm::vec4(gameObject->transform.right, 0);
        glm::vec4 up = glm::vec4(gameObject->transform.up, 0);

        matrix = glm::rotate(matrix, pitch, gameObject->transform.right);
        matrix *= glm::rotate(matrix, -yaw, gameObject->transform.up);


        forward = matrix*forward;
        up = matrix*up;
        right = matrix*right;

        gameObject->transform.forward.x = forward.x;
        gameObject->transform.forward.y = forward.y;
        gameObject->transform.forward.z = forward.z;

        //gameObject->transform.up.x = up.x;
        //gameObject->transform.up.y = up.y;
        //gameObject->transform.up.z = up.z;

        gameObject->transform.right.x = right.x;
        gameObject->transform.right.y = right.y;
        gameObject->transform.right.z = right.z;



        /*glm::vec3 front=glm::vec3(0,0,0);
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch)); 
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        gameObject->transform.forward = glm::normalize(front);
        gameObject->transform.right = glm::normalize(glm::cross(gameObject->transform.forward, gameObject->transform.up));
        gameObject->transform.up = glm::normalize(glm::cross(gameObject->transform.forward, gameObject->transform.right));*/

	}

    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    xoffset = (float)xpos - lastX;
    yoffset = lastY - (float)ypos;
    lastX = (float)xpos;
    lastY = (float)ypos;

	//... Petter - WASD Movement
	float cameraSpeed = 0.03f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gameObject->transform.position += cameraSpeed * gameObject->transform.forward;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gameObject->transform.position -= cameraSpeed * gameObject->transform.forward;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gameObject->transform.position -= gameObject->transform.right * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gameObject->transform.position += gameObject->transform.right * cameraSpeed;
    

	//... Query locate and send in view_matrix
    //cameraPos = gameObject->transform.position;
}
