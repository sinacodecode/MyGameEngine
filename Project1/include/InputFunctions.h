#ifndef INPUTFUNCTIONS_H
#define INPUTFUNCTIONS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "ConfigValues.h"
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);
//

//needs serious cleanup

class InputFunctions
{
public:
    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    static void processInput(GLFWwindow* window)
    {
        ImGuiIO& io = ImGui::GetIO();

        int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);
        if (io.WantCaptureMouse && cursorMode == GLFW_CURSOR_NORMAL || Rendering::isPaused)
        {
            return;
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Rendering::camera.ProcessKeyboard(FORWARD, Rendering::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Rendering::camera.ProcessKeyboard(BACKWARD, Rendering::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Rendering::camera.ProcessKeyboard(LEFT, Rendering::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Rendering::camera.ProcessKeyboard(RIGHT, Rendering::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            Rendering::rotator += 0.01f;
        bool pIsPressed = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
        if (pIsPressed && !KeyEvents::pWasPressed) {
            // This block only runs ONCE per tap
            Rendering::isPaused = !Rendering::isPaused;
            ToggleInputMode(window);
            KeyEvents::pWasPressed = pIsPressed; // Update previous state

        }
        //KeyEvents::pWasPressed = pIsPressed; // Update previous state
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        // 1. Let ImGui handle its own keyboard input first
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureKeyboard) return;

        // 2. Handle our custom 'P' toggle
        if (key == GLFW_KEY_P && action == GLFW_PRESS)
        {
            Rendering::isPaused = !Rendering::isPaused;
            ToggleInputMode(window);
        }

        // You can also move your Escape/Close logic here
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
    // glfw: whenever the window size changed (by OS or user resize) this callback function executes 
    // ---------------------------------------------------------------------------------------------
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
    {
        // 1. Get the ImGui IO object
        ImGuiIO& io = ImGui::GetIO();

        // 2. CHECK: If ImGui wants the mouse, ignore this event for the camera
        // We only do this if the cursor is NORMAL (UI mode). 
        // If it's DISABLED (Game mode), we usually want the camera to move.
        int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);
        if (io.WantCaptureMouse && cursorMode == GLFW_CURSOR_NORMAL || Rendering::isPaused)
        {
            return;
        }

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (Rendering::firstMouse)
        {
            Rendering::lastX = xpos;
            Rendering::lastY = ypos;
            Rendering::firstMouse = false;
        }

        float xoffset = xpos - Rendering::lastX;
        float yoffset = Rendering::lastY - ypos; // reversed since y-coordinates go from bottom to top

        Rendering::lastX = xpos;
        Rendering::lastY = ypos;

        Rendering::camera.ProcessMouseMovement(xoffset, yoffset);
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        ImGuiIO& io = ImGui::GetIO();

        int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);
        if (io.WantCaptureMouse && cursorMode == GLFW_CURSOR_NORMAL || Rendering::isPaused)
        {
            return;
        }

        Rendering::camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }

    static void ToggleInputMode(GLFWwindow* window) {
        if (Rendering::isPaused) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Rendering::firstMouse = true; // Prevents camera jump
        }
    }
};
#endif 

