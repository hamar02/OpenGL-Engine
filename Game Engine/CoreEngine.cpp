#include "CoreEngine.h"

CoreEngine::CoreEngine()
{
	//No shaders here
	window = glfwCreateWindow(1280, 720, "Game Engine", NULL, NULL);
	glfwMakeContextCurrent(window);
	window2 = glfwCreateWindow(1280, 720, "Editor", NULL, window);
	
	materialLibrary = MaterialLib(&assetManager);
	textureLibrary = TextureLib(&assetManager);
	meshLibrary = MeshLib(&assetManager);

}

CoreEngine::~CoreEngine()
{
}

void CoreEngine::Run()
{
		//glfwSwapInterval(1); // Enable vsync
		gl3wInit();

		glm::vec3 e = glm::vec3(90, 90, 90);
		glm::quat q = glm::quat(e);
		glm::vec3 res = glm::eulerAngles(q);
		int iff = 0;
		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Read 'extra_fonts/README.txt' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//ImGuiIO& io = ImGui::GetIO();
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
		//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);
		EditorGUI editorGUI = EditorGUI(window, &gameScene);



		///////////////
		GLuint renderBuffer;
		GLuint frameBuffer;
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		
		glGenTextures(1, &renderBuffer);
		glBindTexture(GL_TEXTURE_2D, renderBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		//attach texture to current framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);

		float oldTime = clock();
		float deltaTime = 0;

		Camera playerCam;
        RenderManager renderManager = RenderManager(&gameScene, window);
		GBufferShaderProgram gBufferShaderProgram = GBufferShaderProgram();
		shadowMapShaderProgram shadowMapProgram;
		gameScene.addEmptyGameObject();
        gameScene.addEmptyGameObject(glm::vec3(90, 0, 0));
        gameScene.addEmptyGameObject(glm::vec3(-5, 2, 0));
        gameScene.addEmptyGameObject();

		//shader måste skapas innan mesh och material
		///////////////////////// Contexts  /////////////////////////////
		glfwMakeContextCurrent(window2);
		GBufferShaderProgram gBufferShaderProgram2 = GBufferShaderProgram();
		ToWindowShaderProgram toWindowShaderProgram2 = ToWindowShaderProgram();
		DrawLineShaderProgram drawLineShaderProgram2 = DrawLineShaderProgram();
		Mesh ndcFrustum2 = Mesh("NDCFrustum.obj",0, gBufferShaderProgram2.gShaderProgram);
		MeshFilter ndcFrustumFilter2 = MeshFilter(ndcFrustum2.gVertexBuffer, ndcFrustum2.gVertexArray, ndcFrustum2.gElementBuffer, ndcFrustum2.vertexCount);
		GLuint error = glGetError();


		glfwMakeContextCurrent(window);
		ToWindowShaderProgram toWindowShaderProgram1 = ToWindowShaderProgram();
		DrawLineShaderProgram drawLineShaderProgram1 = DrawLineShaderProgram();
		Mesh ndcFrustum1 = Mesh("NDCFrustum.obj", 0, gBufferShaderProgram.gShaderProgram);
		MeshFilter ndcFrustumFilter1 = MeshFilter(ndcFrustum1.gVertexBuffer, ndcFrustum1.gVertexArray, ndcFrustum1.gElementBuffer, ndcFrustum1.vertexCount);
		////////////////

        Light dirLight = Light(shadowMapProgram.gShaderProgram, LIGHT_TYPES::Directional);
		Light pointLight = Light(shadowMapProgram.gShaderProgram, LIGHT_TYPES::Point);

		CharacterMovement moveScript = CharacterMovement(window);
        Camera mainCamera = Camera();

		////
		meshLibrary.addMesh("Plane.obj", gBufferShaderProgram.gShaderProgram);
		textureLibrary.addTexture("Cube_cube_BaseColor.png");
		textureLibrary.addTexture("Cube_cube_Normal.png");
		textureLibrary.addTexture("Cube_cubeDIELECTRIC_OcclusionRoughnessMetallic.png");
		textureLibrary.addTexture("Cube_cubeIRON_OcclusionRoughnessMetallic.png");

		//Add material
		///////////
		materialLibrary.addMaterial(gBufferShaderProgram.gShaderProgram);
		//Add Texture
		materialLibrary.getMaterial(0)->setAlbedoTexture(textureLibrary.getTexture(0)->gTexture);
		materialLibrary.getMaterial(0)->setNormalTexture(textureLibrary.getTexture(1)->gTexture);
		materialLibrary.getMaterial(0)->setAORoughMetTexture(textureLibrary.getTexture(2)->gTexture);
		//materialLibrary.getMaterial(0)->setAORoughMetTexture(textureLibrary.getTexture(3)->gTexture);
		MeshFilter meshFilter = MeshFilter(meshLibrary.getMesh(0)->gVertexBuffer, meshLibrary.getMesh(0)->gVertexArray, meshLibrary.getMesh(0)->gElementBuffer, meshLibrary.getMesh(0)->vertexCount);
		gameScene.gameObjects[0]->addComponent(&meshFilter);
		gameScene.gameObjects[0]->addComponent(materialLibrary.getMaterial(0));

        gameScene.gameObjects[1]->addComponent(&meshFilter);
        gameScene.gameObjects[1]->addComponent(materialLibrary.getMaterial(0));

		gameScene.gameObjects[2]->name = "Camera1";
        gameScene.gameObjects[2]->addComponent(&moveScript);
        gameScene.gameObjects[2]->addComponent(&mainCamera);
		gameScene.mainCamera = gameScene.gameObjects[2];

        gameScene.gameObjects[3]->name = "dirLight";
        gameScene.gameObjects[3]->addComponent(&dirLight);
        gameScene.gameObjects[3]->addComponent(materialLibrary.getMaterial(0));

		gameScene.addEmptyGameObject(glm::vec3(0, 0.5, 0));
		gameScene.gameObjects[4]->name = "OBJ Import";
		
		//OBJ Import
		int meshID = 0;
		int matID = 0;
		importOBJFile("Cube.obj", gBufferShaderProgram.gShaderProgram,meshID, matID);
		MeshFilter meshFilter1 = MeshFilter(meshLibrary.getMesh(meshID)->gVertexBuffer, meshLibrary.getMesh(meshID)->gVertexArray, meshLibrary.getMesh(meshID)->gElementBuffer, meshLibrary.getMesh(meshID)->vertexCount);
		gameScene.gameObjects[4]->addComponent(&meshFilter1);
		gameScene.gameObjects[4]->addComponent(materialLibrary.getMaterial(matID));
		//std::cout << "bilden " <<meshID<<matID << std::endl;

		gameScene.addEmptyGameObject(glm::vec3(0,-2,0));
		gameScene.gameObjects[5]->name = "pointLight";
		gameScene.gameObjects[5]->addComponent(&pointLight);
		gameScene.gameObjects[5]->addComponent(materialLibrary.getMaterial(0));

		gameScene.addEmptyGameObject(glm::vec3(-8, 2, 0));
		CharacterMovement moveScript2 = CharacterMovement(window2);
		Camera mainCamera2 = Camera();
		gameScene.gameObjects[6]->name = "Camera2";
		gameScene.gameObjects[6]->addComponent(&moveScript2);
		gameScene.gameObjects[6]->addComponent(&mainCamera2);



		glEnable(GL_DEPTH_TEST);

		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		FrustumCullingShaderProgram frustumCS = FrustumCullingShaderProgram();
		int display_w;
		int display_h;
		float currentTime = 0;
		//////////////
		// Main loop
		error = glGetError();

		while (!glfwWindowShouldClose(window))
		{
			//Update Delta Time
			currentTime = clock();
			deltaTime = currentTime - oldTime;
			oldTime = currentTime;


			//Update Gameobjects
			for (int objectIndex = 0; objectIndex < gameScene.gameObjects.size(); objectIndex++)
			{
				for (int componentIndex = 0; componentIndex < gameScene.gameObjects[objectIndex]->components.size(); componentIndex++)
				{
					gameScene.gameObjects[objectIndex]->components[componentIndex]->update();
				}
			}

			//Find Gameobjects with light component
			FindObjectsToRender();
			
			//Find Gameobjects to Render
			glfwGetFramebufferSize(window, &display_w, &display_h);
			std::vector<GameObject*>objectsToRender = frustumCS.activateShaderProgram(gameScene.gameObjects, gameScene.mainCamera, display_w, display_h);

			
			error = glGetError();

			// Rendering
			glfwMakeContextCurrent(window);
			renderManager.defferedRender(frameBuffer, window,gameScene.mainCamera, objectsToRender, lightsToRender);
			error = glGetError();
			toWindowShaderProgram1.draw(renderBuffer);
			error = glGetError();
			error = glGetError();

			//glDisable(GL_CULL_FACE);
			//drawLineShaderProgram1.drawObject(&ndcFrustumFilter1, gameScene.mainCamera, gameScene.mainCamera, display_w, display_h);
			//glEnable(GL_CULL_FACE);
			editorGUI.showGUI();
			ImGui::Render();
			glfwSwapBuffers(window);
			error = glGetError();


			renderManager.defferedRender(frameBuffer, window2, gameScene.gameObjects[6], objectsToRender, lightsToRender);
			glDisable(GL_CULL_FACE);
			drawLineShaderProgram1.drawObject(&ndcFrustumFilter1, gameScene.mainCamera, gameScene.gameObjects[6], display_w, display_h);
			glEnable(GL_CULL_FACE);
			glfwMakeContextCurrent(window2);
			toWindowShaderProgram2.draw(renderBuffer);
			glfwSwapBuffers(window2);


			//Clean up
			error = glGetError();
			lightsToRender.clear();
			glfwMakeContextCurrent(window);

			

		}

		// Cleanup
		ImGui_ImplGlfwGL3_Shutdown();
		glfwTerminate();
}

int CoreEngine::importMTLFile(std::string filepath, unsigned int shaderProgram)
{
	int indexMaterial = materialLibrary.addMaterial(shaderProgram);
	glm::vec3 albedoRGB = glm::vec3(0, 0, 0);
	glm::vec3 aoRoughMetRGB = glm::vec3(1, 0, 0);
	int indexTexture = -1;
	const char *meshNameChar = filepath.c_str();
	FILE *file = fopen(meshNameChar, "r");
	while (file != NULL)
	{
		char lineHeader[128];
		int word = fscanf(file, "%s", lineHeader);
		if (word == EOF) //EOF = End Of Line.
		{
			break;
		}
		else if (strcmp(lineHeader, "mtllib") == 0)
		{
			char mtlChar[128];
			fscanf(file, "%s", mtlChar);
			FILE* mtlFile = fopen(mtlChar, "r");

			while (true)
			{
				char mtlLineHeader[128];
				int mtlWord = fscanf(mtlFile, "%s", mtlLineHeader);
				if (mtlWord == EOF)
				{
					break;
				}
				else if (strcmp(mtlLineHeader, "Kd") == 0)
				{
					float x, y, z = 0;
					fscanf(mtlFile, "%f %f %f", &x, &y, &z);
					albedoRGB.x = x;
					albedoRGB.y = y;
					albedoRGB.z = z;
				}
				else if (strcmp(mtlLineHeader, "Ka") == 0)
				{
					float x, y, z = 0;
					fscanf(mtlFile, "%f %f %f", &x, &y, &z);
					x = 1 - x;
					y = 1 - y;
					z = 1 - z;

					float maxV = std::min(x, y);
					maxV = std::min(maxV, z);

					aoRoughMetRGB.r = maxV;
				}
				else if (strcmp(mtlLineHeader, "Ks") == 0)
				{
					float x, y, z = 0;
					fscanf(mtlFile, "%f %f %f", &x, &y, &z);
					float maxV = std::max(x, y);
					maxV = std::max(maxV, z);
					//std::cout << "debug" << x << y << z << std::endl;
					aoRoughMetRGB.g = maxV;
				}
				else if (strcmp(mtlLineHeader, "map_Kd") == 0)
				{
					char textureFile[128];
					fscanf(mtlFile, "%s", textureFile);
					indexTexture = textureLibrary.addTexture(std::string(textureFile));

				}
			}
		}
	}

	if (indexTexture != -1)
	{
		materialLibrary.getMaterial(indexMaterial)->setAlbedoTexture(textureLibrary.getTexture(indexTexture)->gTexture);
	}
	materialLibrary.getMaterial(indexMaterial)->setAlbedoRGB(albedoRGB);
	materialLibrary.getMaterial(indexMaterial)->setAORoughMetRGB(aoRoughMetRGB);

	return indexMaterial;
}

void CoreEngine::importOBJFile(std::string filepath, unsigned int shaderProgram, int &meshID, int &materialID)
{
	meshID = meshLibrary.addMesh(filepath, shaderProgram);
	materialID = importMTLFile(filepath, shaderProgram);
	//std::cout << "return " << meshID << materialID << std::endl;
}



void CoreEngine::FindObjectsToRender() {
	for (int i = 0; i < gameScene.gameObjects.size(); i++) {
		if (gameScene.gameObjects[i]->hasLight == true) {
			lightsToRender.push_back(gameScene.gameObjects[i]);
		}
	}
}

