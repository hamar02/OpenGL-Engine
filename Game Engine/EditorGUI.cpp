#include "EditorGUI.h"

EditorGUI::EditorGUI()
{

}

EditorGUI::EditorGUI(GLFWwindow * otherWindow, GameScene * otherGameScene)
{
	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(otherWindow, true);

	gameScene = otherGameScene;
	window = otherWindow;

	// Setup style
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();

}

EditorGUI::~EditorGUI()
{
}

void EditorGUI::showGUI()
{
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	glfwPollEvents();
	ImGui_ImplGlfwGL3_NewFrame();


	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		ImGui::Text("Hello, world!");                           // Some text (you can use a format string too)
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats as a color
		if (ImGui::Button("Demo Window"))                       // Use buttons to toggle our bools. We could use Checkbox() as well.
			show_demo_window ^= 1;
		if (ImGui::Button("Another Window"))
			show_another_window ^= 1;
		if (ImGui::Button("Hierarchy Window"))
			show_hierarchy_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name the window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello from another window!");
		ImGui::End();
	}

	//hierarchy window
	if (show_hierarchy_window)
	{
		ImGui::Begin("Hierarchy", &show_hierarchy_window);
		ImGui::Text("Hierarchy");

		static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
		for (int i = 0; i < gameScene->gameObjects.size(); i++)
		{
			// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);

			{
				// Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
				std::string gameObjectName = gameScene->gameObjects[i]->name;
				const char * c = gameObjectName.c_str();

				ImGui::TreeNodeEx(c, node_flags, "%s", c);
				if (ImGui::IsItemClicked())
					gameObject_clicked = i;
			}
		}
		if (gameObject_clicked != -1)
		{
			// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
			if (ImGui::GetIO().KeyCtrl)
				selection_mask ^= (1 << gameObject_clicked);          // CTRL+click to toggle
			else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
				selection_mask = (1 << gameObject_clicked);           // Click to single-select
		}
		ImGui::PopStyleVar();


		ImGui::End();
	}
	//std::cout << (gameObject_clicked) << std::endl;

	//Inspector window
	if (show_inspector_window && gameObject_clicked != -1)
	{
		ImGui::Begin("Inspector", &show_inspector_window);
		ImGui::Text("Inspector");

		static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
		int component_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.


																					//start transform
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << 0)) ? ImGuiTreeNodeFlags_Selected : 0);

		// Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
		node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << 0)) ? ImGuiTreeNodeFlags_Selected : 0);
		std::string componentName = "Transform";

		const char * c = componentName.c_str();

		bool node_open = ImGui::TreeNodeEx(c, node_flags, "%s", c);
		if (ImGui::IsItemClicked())
			component_clicked = 0;
		if (node_open)
		{
			ImGui::SliderFloat("X Pos", &gameScene->gameObjects[gameObject_clicked]->transform.position.x, -10.0f, 10.0f);            // Edit 1 float as a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Y Pos", &gameScene->gameObjects[gameObject_clicked]->transform.position.y, -10.0f, 10.0f);            // Edit 1 float as a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Z Pos", &gameScene->gameObjects[gameObject_clicked]->transform.position.z, -10.0f, 10.0f);            // Edit 1 float as a slider from 0.0f to 1.0f
			ImGui::SliderFloat("X Rot", &gameScene->gameObjects[gameObject_clicked]->transform.rotation.x, -90.0f, 90.0f);            // Edit 1 float as a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Y Rot", &gameScene->gameObjects[gameObject_clicked]->transform.rotation.y, -90.0f, 90.0f);            // Edit 1 float as a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Z Rot", &gameScene->gameObjects[gameObject_clicked]->transform.rotation.z, -90.0f, 90.0f);            // Edit 1 float as a slider from 0.0f to 1.0f
			gameScene->gameObjects[gameObject_clicked]->transform.updateRotationInEuler();
			//std::cout << "Rot In euler" << gameScene->gameObjects[gameObject_clicked]->transform.rotation.x << gameScene.gameObjects[gameObject_clicked]->transform.rotation.y << gameScene.gameObjects[gameObject_clicked]->transform.rotation.z << std::endl;
			ImGui::Text("Empty object is empty");
			ImGui::TreePop();
		}
		//end transform
		if (ImGui::IsItemClicked())
			component_clicked = 0;

		static bool options_menu = true;
		static bool alpha_preview = true;
		static bool alpha_half_preview = false;
		static bool hdr = false;
		static ImVec4 color = ImColor(114, 144, 154, 200);

		int misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

		for (int i = 0; i < gameScene->gameObjects[gameObject_clicked]->components.size(); i++)
		{
			// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);

			// Node
			std::string componentName = gameScene->gameObjects[gameObject_clicked]->components[i]->assetName;
			const char * c = componentName.c_str();

			node_open = ImGui::TreeNodeEx(c, node_flags, "%s", c);
			if (ImGui::IsItemClicked())
				component_clicked = i;
			if (node_open)
			{
				Light* lightGUI = gameScene->gameObjects[gameObject_clicked]->lightComponent;
				if (lightGUI != nullptr) {
					ImGui::Text("Color button with Picker:");
					ImGui::ColorEdit4("Color##1", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
					glm::vec4 tempColor = glm::vec4(color.x, color.y, color.z, color.w);
					lightGUI->color = tempColor;
					ImGui::SliderFloat("Intensity", &gameScene->gameObjects[gameObject_clicked]->lightComponent->intensity, 0.0f, 100.0f);            // Edit 1 float as a slider from 0.0f to 1.0f

				}
				ImGui::Text("Add Stuff Here");
				ImGui::TreePop();
			}


		}
		if (component_clicked != -1)
		{
			// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
			if (ImGui::GetIO().KeyCtrl)
				selection_mask ^= (1 << component_clicked);          // CTRL+click to toggle
			else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
				selection_mask = (1 << component_clicked);           // Click to single-select
		}
		ImGui::PopStyleVar();


		ImGui::End();
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow().
	if (show_demo_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&show_demo_window);
	}

}

