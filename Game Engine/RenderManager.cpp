#include "RenderManager.h"



RenderManager::RenderManager() {

}

RenderManager::RenderManager(GameScene * otherGameScene, GLFWwindow* otherWindow) {
	gameScene = otherGameScene;
	glfwGetFramebufferSize(otherWindow, &display_w, &display_h);

	createGBuffer();
	createLPBuffer();
	lightingShaderProgram = LightingPassShaderProgram();


}

RenderManager::~RenderManager() {
}

void RenderManager::defferedRender(GLuint outFBO, GLFWwindow* otherWindow, GameObject* camera, std::vector<GameObject*> &gameObjectsToRender, std::vector<GameObject*> &lightsToRender) {
	glfwGetFramebufferSize(otherWindow, &display_w, &display_h);

	//Shadow Mapping
	glCullFace(GL_BACK);
	for (int i = 0; i < lightsToRender.size(); i++)
	{
		//directionalLightsToRender[i]->uniformWorldMatrix(dT);
		lightsToRender[i]->lightComponent->setupShadowMap();
		unsigned int lightShaderProgram= lightsToRender[i]->lightComponent ->gShaderProgram;
		for (int j = 0; j < gameObjectsToRender.size(); j++) {
			lightsToRender[i]->lightComponent->uniformLightSpaceMatrix(lightShaderProgram);
			gameObjectsToRender[j]->meshFilterComponent->bindVertexArray();
			gameObjectsToRender[j]->uniformWorldMatrix(lightShaderProgram);
			glDrawElements(GL_TRIANGLES, gameObjectsToRender[j]->meshFilterComponent->vertexCount, GL_UNSIGNED_INT, 0);
		}
	}

	//Geometry pass
	glCullFace(GL_BACK);

	glViewport(0, 0, display_w, display_h);
	glBindFramebuffer(GL_FRAMEBUFFER, gbo);
	glClearColor(0.0, 0.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);
	Camera* mainCamera = camera->getComponent<Camera>();
	glm::mat4x4 projection_matrix = mainCamera->calculatePerspectiveMatrix(display_w, display_h);
	glm::mat4x4 view_matrix = mainCamera->calculateViewMatrix();
    for (int i = 0; i < gameObjectsToRender.size(); i++) {
        //std::cout << "update"<< world_position.x << std::endl;

        // tell opengl we want to use the gShaderProgram
		gameObjectsToRender[i]->materialComponent->activateShaderProgram();

		//Load in Textures
		gameObjectsToRender[i]->materialComponent->bindTextures();
		
		// tell opengl we are going to use the VAO we described earlier
        gameObjectsToRender[i]->meshFilterComponent->bindVertexArray();

        //Tranformation
        //Matrices
        GLuint uniformLoc = glGetUniformLocation(gameObjectsToRender[i]->materialComponent->gShaderProgram, "projection_matrix");
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		gameObjectsToRender[i]->uniformWorldMatrix(gameObjectsToRender[i]->materialComponent->gShaderProgram);
		
		uniformLoc = glGetUniformLocation(gameObjectsToRender[i]->materialComponent->gShaderProgram, "view_matrix");
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glDrawElements(GL_TRIANGLES, gameObjectsToRender[i]->meshFilterComponent->vertexCount, GL_UNSIGNED_INT, 0);

    }

	//SSAO
	ssaoShaderProgram.activateShaderProgram();
	glUniform1i(glGetUniformLocation(ssaoShaderProgram.gShaderProgram, "gPosition"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);

	glUniform1i(glGetUniformLocation(ssaoShaderProgram.gShaderProgram, "gNormal"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);

	glUniform1i(glGetUniformLocation(ssaoShaderProgram.gShaderProgram, "texNoise"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, ssaoShaderProgram.noiseTexture);

	GLuint uniformLoc = glGetUniformLocation(ssaoShaderProgram.gShaderProgram, "projection");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	
	for (unsigned int i = 0; i < 64; ++i) {
		std::string uniformName = "samples[" + std::to_string(i) + "]";
		GLuint loc = glGetUniformLocation(ssaoShaderProgram.gShaderProgram, uniformName.c_str());
		glUniform3fv(loc, 1, glm::value_ptr(ssaoShaderProgram.ssaoKernel[i]));

	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//SSAO Blur
	postEffectShaderProgram.activateShaderProgram();

	int pingPong = true;
	for (int i = 0; i < 0; i++)
	{
		GLint loc = glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "horizontal");
		glUniform1i(loc, pingPong);

		loc = glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "colorChannels");
		glUniform1i(loc, 1);

		if (pingPong == true)
		{
			glUniform1i(glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "inImage"), 0);
			glBindImageTexture(0, ssaoShaderProgram.ssaoColorBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

			glUniform1i(glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "outImage"), 1);
			glBindImageTexture(1, lProcessedColor, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

			glDispatchCompute(display_w / 32, (display_h / 30), 1);
			pingPong = false;
		}
		else {
			glUniform1i(glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "inImage"), 0);
			glBindImageTexture(0, lProcessedColor, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

			glUniform1i(glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "outImage"), 1);
			glBindImageTexture(1, ssaoShaderProgram.ssaoColorBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

			glDispatchCompute(display_w / 32, (display_h / 30), 1);
			pingPong = true;

		}

	}

		
	//Lighting Pass
	glCullFace(GL_BACK);
	glm::mat4 invView_matrix = glm::inverse(view_matrix);
	lightingShaderProgram.activateShaderProgram();

	uniformLoc = glGetUniformLocation(lightingShaderProgram.screenQuadShader, "invView_matrix");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(invView_matrix));

	uniformLoc = glGetUniformLocation(lightingShaderProgram.screenQuadShader, "view_matrix");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));


	//CAM pos
	uniformLoc = glGetUniformLocation(lightingShaderProgram.screenQuadShader, "view_position");
	glUniform3fv(uniformLoc, 1, glm::value_ptr(gameScene->mainCamera->transform.position));


	glUniform1i(glGetUniformLocation(lightingShaderProgram.screenQuadShader, "gPosition"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);

	glUniform1i(glGetUniformLocation(lightingShaderProgram.screenQuadShader, "gNormal"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);

	glUniform1i(glGetUniformLocation(lightingShaderProgram.screenQuadShader, "gAlbedo"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);

	glUniform1i(glGetUniformLocation(lightingShaderProgram.screenQuadShader, "gAORoughMet"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gAORoughMet);

	glUniform1i(glGetUniformLocation(lightingShaderProgram.screenQuadShader, "gEmissive"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gEmissive);

	glUniform1i(glGetUniformLocation(lightingShaderProgram.screenQuadShader, "gSSAO"), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, ssaoShaderProgram.ssaoColorBuffer);

	glUniform1i(glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "FragColor"), 6);
	glBindImageTexture(6, lColor, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	glUniform1i(glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "FragHDRColor"), 7);
	glBindImageTexture(7, lHDRColor, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	int index = 8;
	
	//Lights
	for (int i = 0; i < lightsToRender.size(); i++)
	{
		std::cout << "lights" << lightsToRender.size() << std::endl;
		std::string lightUniform;
		
		//Type
		lightUniform = "lights[" + std::to_string(i) + "].lightType";
		GLuint loc = glGetUniformLocation(lightingShaderProgram.screenQuadShader, lightUniform.c_str());
		glUniform1i(loc, GLint(lightsToRender.at(i)->lightComponent->lightType));

		//Position
		lightUniform = "lights[" + std::to_string(i) + "].Position";
		loc = glGetUniformLocation(lightingShaderProgram.screenQuadShader, lightUniform.c_str());
		glUniform3fv(loc, 1, glm::value_ptr(lightsToRender.at(i)->transform.position));

		//Direction
		lightUniform = "lights[" + std::to_string(i) + "].DirectionVector";
		loc = glGetUniformLocation(lightingShaderProgram.screenQuadShader, lightUniform.c_str());
		glUniform3fv(loc, 1, glm::value_ptr(lightsToRender.at(i)->transform.up));

		//Color
		lightUniform = "lights[" + std::to_string(i) + "].Color";
		loc = glGetUniformLocation(lightingShaderProgram.screenQuadShader, lightUniform.c_str());
		glUniform3fv(loc, 1, glm::value_ptr(lightsToRender.at(i)->lightComponent->color));
		//std::cout << " Color" << std::endl << directionalLightsToRender.at(i)->lightComponent->color.x << directionalLightsToRender.at(i)->lightComponent->color.y << directionalLightsToRender.at(i)->lightComponent->color.z << std::endl;

		//Intensity
		lightUniform = "lights[" + std::to_string(i) + "].Intensity";
		loc = glGetUniformLocation(lightingShaderProgram.screenQuadShader, lightUniform.c_str());
		glUniform1f(loc, lightsToRender.at(i)->lightComponent->intensity);

		//Range
		lightUniform = "lights[" + std::to_string(i) + "].Range";
		loc = glGetUniformLocation(lightingShaderProgram.screenQuadShader, lightUniform.c_str());
		glUniform1f(loc, lightsToRender.at(i)->lightComponent->range);


		//Light space matrix
		lightUniform = "lights[" + std::to_string(i) + "].lightSpace_matrix";
		loc = glGetUniformLocation(lightingShaderProgram.screenQuadShader, lightUniform.c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(lightsToRender.at(i)->lightComponent->lightSpaceMatrix));
		
		//Shadow depthmap
		lightUniform = "lights[" + std::to_string(i) + "].shadowMap";
		glUniform1i(glGetUniformLocation(lightingShaderProgram.screenQuadShader, lightUniform.c_str()), index);
		glActiveTexture(GL_TEXTURE0+index);
		glBindTexture(GL_TEXTURE_2D, lightsToRender.at(i)->lightComponent->depthMap);

		index++;

	}
	//glMemoryBarrier(GL_ALL_BARRIER_BITS);
	GLint error = glGetError();
	glDispatchCompute(display_w / 32, (display_h / 30), 1);
	error = glGetError();
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	//Post Effects
	postEffectShaderProgram.activateShaderProgram();

	pingPong = true;
	for (int i = 0; i < 10; i++)
	{
		GLint loc = glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "horizontal");
		glUniform1i(loc, pingPong);

		loc = glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "colorChannels");
		glUniform1i(loc, 3);

		if (pingPong==true)
		{
			glUniform1i(glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "inImage"), 0);
			glBindImageTexture(0, lHDRColor, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

			glUniform1i(glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "outImage"), 1);
			glBindImageTexture(1, lProcessedColor, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
			
			glDispatchCompute(display_w / 32, (display_h / 30), 1);
			
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			pingPong = false;
		}
		else {
			glUniform1i(glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "inImage"), 0);
			glBindImageTexture(0, lProcessedColor, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

			glUniform1i(glGetUniformLocation(postEffectShaderProgram.gShaderProgram, "outImage"), 1);
			glBindImageTexture(1, lHDRColor, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
			glDispatchCompute(display_w / 32, (display_h / 30), 1);

			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			pingPong = true;

		}

	}
	GLint hej = glGetError();


	//Render To Screen, yay!
	glBindFramebuffer(GL_FRAMEBUFFER, outFBO);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(MergeBloomShaderProgram.gShaderProgram);
	
	glUniform1f(glGetUniformLocation(MergeBloomShaderProgram.gShaderProgram, "exposure"), 1.0);

	glUniform1i(glGetUniformLocation(MergeBloomShaderProgram.gShaderProgram, "textureColorIn"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lColor);

	glUniform1i(glGetUniformLocation(MergeBloomShaderProgram.gShaderProgram, "textureBloomIn"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lHDRColor);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void RenderManager::createGBuffer()
{
	//framebufferobject
	glGenFramebuffers(1, &gbo);
	glBindFramebuffer(GL_FRAMEBUFFER, gbo);

	//g-buffer position
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, display_w, display_h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	//attach texture to current framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	//g-buffer normal
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, display_w, display_h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	//attach texture to current framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	//g-buffer albedo
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, display_w, display_h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	//attach texture to current framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);

	//g-buffer specular + metallic + AO
	glGenTextures(1, &gAORoughMet);
	glBindTexture(GL_TEXTURE_2D, gAORoughMet);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, display_w, display_h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	//attach texture to current framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gAORoughMet, 0);

	//Emmisive
	glGenTextures(1, &gEmissive);
	glBindTexture(GL_TEXTURE_2D, gEmissive);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, display_w, display_h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	//attach texture to current framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gEmissive, 0);


	glDrawBuffers(5, gAttachments);


	//renderbufferobject
	glGenRenderbuffers(1, &gRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, gRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, display_w, display_h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//attach renderbufferobject to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gRbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::GBUFFER:: Framebuffer is not complete! " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
		{
			std::cout << "Attachment" << std::endl;

		}

	}
	else
	{
		std::cout << "SUCCESS::FRAMEBUFFER:: Framebuffer is complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderManager::createLPBuffer()
{
	glGenFramebuffers(1, &LPfbo);
	glBindFramebuffer(GL_FRAMEBUFFER, LPfbo);

	//Color attachments
	glGenTextures(1, &lColor);
	glBindTexture(GL_TEXTURE_2D, lColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, display_w, display_h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lColor, 0);

	glGenTextures(1, &lHDRColor);
	glBindTexture(GL_TEXTURE_2D, lHDRColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, display_w, display_h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, lHDRColor, 0);

	glGenTextures(1, &lProcessedColor);
	glBindTexture(GL_TEXTURE_2D, lProcessedColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, display_w, display_h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, lProcessedColor, 0);

	glDrawBuffers(3, pAttachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
