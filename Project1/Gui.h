#ifndef GUI_H
#define GUI_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "SceneObject.h"
#include "camera.h"

class Gui
{
public:
	Gui(GLFWwindow* window)
		:m_window{window}
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
		if (!m_window)
			std::cout << "m_window is null!\n";
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(m_window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init();
	}
	void drawObject(Object& object, Camera& camera, Light& light);
	void newWindow();
	void renderWindow();
	bool closable_group{ true };
private:
	//ImGuiIO& io = ImGui::GetIO();
	GLFWwindow* m_window;
};

#endif