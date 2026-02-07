#include "Gui.h"

#include <GLFW/glfw3.h>

#include "SceneObject.h"
#include "InputFunctions.h"

void Gui::drawObject(Object& object, Camera& camera, Light& light)
{
    //ImGui::InputFloat("Location x: ", &location.x, 0.0f, 0.0f, "%.1f");
    //ImGui::InputFloat("Location y: ", &location.y, 0.0f, 0.0f, "%.1f");
    //ImGui::InputFloat("Location z: ", &location.z, 0.0f, 0.0f, "%.1f");

    //ImGui::InputFloat("rotation x: ", &rx, 0.0f, 0.0f, "%.1f");
    //ImGui::InputFloat("rotation y: ", &ry, 0.0f, 0.0f, "%.1f");
    //ImGui::InputFloat("rotation z: ", &rz, 0.0f, 0.0f, "%.1f");

    //ImGui::InputFloat("degree: ", &degree, 0.0f, 0.0f, "%.1f");
    ImGuiIO& m_io = ImGui::GetIO();
    m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //m_io.WantCaptureMouse || glfwGetKey(m_window, GLFW_KEY_C)
    //if (glfwGetKey(m_window, GLFW_KEY_C))
    //{
    //    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //    glfwSetCursorPosCallback(m_window, InputFunctions::mouse_callback);
    //    glfwSetScrollCallback(m_window, InputFunctions::scroll_callback);

    //    std::cout << "Ain't gettin' shit\n";

    //}
    //else
    //{
    //    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //    glfwSetCursorPosCallback(m_window, NULL);
    //    glfwSetScrollCallback(m_window, NULL);

    //    std::cout << "gettin' mouse input\n";
    //}

    if (ImGui::TreeNode("Scene"))
    {
        if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("TBA");
            ImGui::Text("Window Pos: (%g, %g)", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
        }
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_None))
        {
            ImGui::InputFloat("light Location x: ", &light.getPosition().x, 0.0f, 0.0f, "%.1f");
            ImGui::InputFloat("light Location y: ", &light.getPosition().y, 0.0f, 0.0f, "%.1f");
            ImGui::InputFloat("light Location z: ", &light.getPosition().z, 0.0f, 0.0f, "%.1f");
            ImGui::InputFloat("linear: ", &light.getLightAttenuation().m_linear, 0.0f, 0.0f, "%.1f");
            ImGui::InputFloat("constant: ", &light.getLightAttenuation().m_constant, 0.0f, 0.0f, "%.1f");
            ImGui::InputFloat("quadratic: ", &light.getLightAttenuation().m_quadratic, 0.0f, 0.0f, "%.1f");
            ImGui::InputFloat("specular: ", &light.getSpecularity(), 0.0f, 0.0f, "%.1f");
        }
        ImGui::TreePop();
    }

    ImGui::Text("mouse Pos: (%g, %g)", ImGui::GetMousePos().x, ImGui::GetMousePos().y);

    ImGui::Render();
    object.draw(camera, light);
}

//#include "InputFunctions.h"
void Gui::newWindow()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    //if (m_io.WantCaptureMouse || glfwGetKey(m_window, GLFW_KEY_F))
    //{
    //    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //}
    //else
    //{
    //    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //    InputFunctions::processInput(m_window);
    //}
}

void Gui::renderWindow()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}