#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/Model.h"
#include <string>
#include "include/shader_s.h"
#include "include/camera.h"
#include <iostream>
#include "stb_image.h"
#include "include/SceneObject.h"
#include "include/Scene.h"
#include "include/Renderer.h"
#include <array>

//#include "ConfigValues.h"
#include "include/InputFunctions.h"
#include "include/Gui.h"


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
    glfwSetKeyCallback(window, InputFunctions::key_callback);
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
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glDepthMask(GL_TRUE);
    //glDepthFunc(GL_ALWAYS);

    Shader ourShader{ "C://Users/Sina/source/repos/sinacodecode/MyGameEngine/Project1/Resources/modelLoader.vs", "C://Users/Sina/source/repos/sinacodecode/MyGameEngine/Project1/Resources/modelLoader.fs"};
    
    Model ourModel{ std::string("Resources/Models/Backpack/Backpack.obj") };
    Model roomModel{ std::string("Resources/Models/Room/Room.obj") };
    //Model ourModel{ std::string("C://Users/Sina/source/repos/sinacodecode/MyGameEngine/Project1/Resources/Models/Backpack/Backpack.obj") };
    //Model roomModel{ std::string("C://Users/Sina/source/repos/sinacodecode/MyGameEngine/Project1/Resources/Models/Room/Room.obj") };

    Light::Color color{ {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} };
    Light::PointLight light1{color, glm::vec3(4.0f, 5.0f, 4.0f) };
    Light::PointLight light2{ color, glm::vec3(2.0f, -1.0f, 1.0f) };
    Light::PointLight light3{ color, glm::vec3(-1.0f, 3.0f, 6.0f) };
    Light::PointLight light4{ color, glm::vec3(7.0f, 3.0f, 4.0f) };
    
    Light::Attenuation atten{};

    Light::DirectionalLight dirlight{ color, {1.0f, 1.0f, 1.0f} };
    Light::SpotLight spotlight{ color, {1.0f, 1.0f, 1.0f}, {1.0f, -1.0f, -1.0f}, atten, 1.0f, 2.0f};


    Object backpack{ourModel};
    Object room{ roomModel };

    std::vector<Object> objects {room, backpack};
    std::vector<Light::LightVariant> lights{light1, light2, dirlight, spotlight};

    //std::vector<Light::LightVariant> lights{ spotlight};

    Scene mainScene{ Rendering::camera, objects, lights };
    mainScene.m_pointLightCount = 4;
    Gui gui{window};

    Renderer renderer{ mainScene , ourShader };
    renderer.render();
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
        ImGuiIO& io = ImGui::GetIO();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            InputFunctions::ToggleInputMode(window);
        }
        // Logic: Only move camera if NOT paused AND ImGui doesn't want the mouse
        InputFunctions::processInput(window);
        gui.newWindow();

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilMask(0xFF); // each bit is written to as is
        glStencilMask(0x00);
        gui.renderScene(renderer);
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
