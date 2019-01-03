#include "DrawLineShaderProgram.h"

DrawLineShaderProgram::DrawLineShaderProgram()
{
	createShaderData();
}

DrawLineShaderProgram::~DrawLineShaderProgram()
{
}


void DrawLineShaderProgram::createShaderData()
{
	ShaderProgram temp = ShaderProgram();
	gShaderProgram = temp.CreateShaderData("LineVS.glsl", "", "", "", "LineFS.glsl", "");
}

void DrawLineShaderProgram::drawObject(MeshFilter* objectToRender, GameObject* mainCamera, GameObject* POVCamera, unsigned int  display_w, unsigned int  display_h)
{
	Camera* mainCameraComponent = mainCamera->getComponent<Camera>();
	glm::mat4x4 projection_matrix = mainCameraComponent->calculatePerspectiveMatrix(display_w, display_h);
	glm::mat4x4 view_matrix = mainCameraComponent->calculateViewMatrix();
	glm::mat4 world_matrix=mainCamera->calculateWorldMatrix();

	glm::mat4 POV_matrix;

	if (POVCamera != mainCamera) {
		Camera*POVCameraComponent = POVCamera->getComponent<Camera>();
		glm::mat4x4 projection_matrix2 = POVCameraComponent->calculatePerspectiveMatrix(display_w, display_h);
		glm::mat4x4 view_matrix2 = POVCameraComponent->calculateViewMatrix();
		POV_matrix = projection_matrix2 * view_matrix2;
	}
	else
	{
		POV_matrix = projection_matrix * view_matrix;
	}

	glUseProgram(gShaderProgram);
	GLuint uniformLoc = glGetUniformLocation(gShaderProgram, "projection_matrix");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	uniformLoc = glGetUniformLocation(gShaderProgram, "view_matrix");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
	uniformLoc = glGetUniformLocation(gShaderProgram, "POV_matrix");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(POV_matrix));
	uniformLoc = glGetUniformLocation(gShaderProgram, "world_matrix");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(world_matrix));
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	objectToRender->bindVertexArray();
	glDrawElements(GL_TRIANGLES, objectToRender->vertexCount, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

