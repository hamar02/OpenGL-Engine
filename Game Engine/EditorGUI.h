#pragma once
#include "GameScene.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"


class EditorGUI
{
public:
	EditorGUI();
	EditorGUI(GLFWwindow* otherWindow, GameScene* otherGameScene);
	~EditorGUI();

	void showGUI();
private:
	GLFWwindow *window;
	GameScene *gameScene;

	bool show_demo_window = true;
	bool show_hierarchy_window = true;
	bool show_inspector_window = true;
	bool show_another_window = false;
	int gameObject_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

};
