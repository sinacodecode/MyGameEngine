#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include <string>
#include "shader_s.h"
#include "camera.h"
#include <iostream>
#include "stb_image.h"
#include "SceneObject.h"
#include <array>

//#include "ConfigValues.h"
#include "InputFunctions.h"
#include "Gui.h"


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif



    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(Rendering::SCR_WIDTH, Rendering::SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, InputFunctions::framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, NULL);
    //glfwSetScrollCallback(window, NULL);

    glfwSetCursorPosCallback(window, InputFunctions::mouse_callback);
    glfwSetScrollCallback(window, InputFunctions::scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    Shader ourShader{ "modelLoader.vs", "modelLoader.fs" };
    Model ourModel{ std::string("C://Users/Sina/source/repos/Project1/Project1/Backpack/Backpack.obj") };
    Light light{ glm::vec3(0.0f, 0.0f, 0.0f) };
    Light::Attenuation atten{};
    Object backpack{ ourShader , ourModel , glm::vec3(0.0f, 0.0f, 0.0f),Rendering::camera , light };
    Gui gui{window};

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        Rendering::deltaTime = currentFrame - Rendering::lastFrame;
        Rendering::lastFrame = currentFrame;

        // input
        // -----
        InputFunctions::processInput(window);
        gui.newWindow();

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilMask(0xFF); // each bit is written to as is
        glStencilMask(0x00);
        gui.drawObject(backpack, Rendering::camera, light);
        gui.renderWindow();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
