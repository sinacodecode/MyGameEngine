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

class InputFunctions
{
public:
    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    static void processInput(GLFWwindow* window)
    {
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
        Rendering::camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
};
#endif 

