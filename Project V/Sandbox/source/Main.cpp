#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Renderer/Model.h>
#include <string>
#include <Renderer/shader_s.h>
#include <Renderer/camera.h>
#include <iostream>
#include <stb_image.h>
#include <Scene/SceneObject.h>
#include <Scene/Scene.h>
#include <Renderer/Renderer.h>
#include <array>

//#include "ConfigValues.h"
#include <Core/InputFunctions.h>
#include <Editor/Gui.h>

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
    GLFWwindow* window = glfwCreateWindow(Rendering::SCR_WIDTH, Rendering::SCR_HEIGHT, "MyGameEngine", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, InputFunctions::framebuffer_size_callback);

    glfwSetKeyCallback(window, InputFunctions::key_callback);
    glfwSetCursorPosCallback(window, InputFunctions::mouse_callback);
    glfwSetScrollCallback(window, InputFunctions::scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << '\n';
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(0);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glDepthMask(GL_TRUE);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<std::unique_ptr<Shader>> shaders;

    shaders.emplace_back(std::make_unique<Shader>("../../Resources/modelLoader.vs", "../../Resources/modelLoader.fs"));
    shaders.emplace_back(std::make_unique<Shader>("../../Resources/outLine.vs", "../../Resources/outLine.fs"));
    shaders.emplace_back(std::make_unique<Shader>("../../Resources/outLine.vs", "../../Resources/rgbaAlphaTransparent.fs"));

    Model ourModel{ std::string("../../Resources/Models/Backpack/Backpack.obj"), 0};
    Model roomModel{ std::string("../../Resources/Models/Room/Room.obj"), 0};

    Light::Color color{ {0.2F, 0.3F, 0.4F}, {0.6F, 0.6F, 0.3F}, {1.0F, 1.0F, 1.0F} };
    Light::PointLight light1{color, glm::vec3(0.4F, 0.5F, 0.4F) };
    Light::PointLight light2{ color, glm::vec3(0.2F, 0.1F, 0.1F) };
    Light::PointLight light3{ color, glm::vec3(0.9F, 0.3F, 0.6F) };
    Light::PointLight light4{ color, glm::vec3(0.3F, 0.7F, 0.4F) };
    
    Light::Attenuation atten{};

    Light::DirectionalLight dirlight{ color, {1.0F, 1.0F, 1.0F} };
    Light::SpotLight spotlight{ color, {1.0F, 1.0F, 1.0F}, {1.0F, -1.0F, -1.0F}, atten, 1.0F, 2.0F};

    std::vector<std::unique_ptr<Object>> objects;
    objects.emplace_back(
        std::make_unique<Object>(
            std::make_unique<Model>(std::move(roomModel)),0
        )
    );

    std::vector<std::unique_ptr<Light::LightVariant>> lights;
    lights.emplace_back(std::make_unique<Light::LightVariant>(light1));
    lights.emplace_back(std::make_unique<Light::LightVariant>(light2));
    lights.emplace_back(std::make_unique<Light::LightVariant>(dirlight));
    lights.emplace_back(std::make_unique<Light::LightVariant>(spotlight));

    std::unique_ptr<Scene> mainScene{ std::make_unique<Scene>(Rendering::camera, std::move(objects), std::move(lights)) };
    mainScene->m_pointLightCount = 4;
    Gui gui{window};

    Renderer renderer{ std::move(mainScene), shaders };

    while (!glfwWindowShouldClose(window))
    {

        auto currentFrame = static_cast<float>(glfwGetTime());
        Rendering::deltaTime = currentFrame - Rendering::lastFrame;
        Rendering::lastFrame = currentFrame;

        ImGuiIO& io = ImGui::GetIO();
       
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            InputFunctions::ToggleInputMode(window);
        }

        InputFunctions::processInput(window);

        glClearColor(0.5F, 0.5F, 0.5F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        gui.newWindow();

        gui.renderScene(renderer);

        renderer.render();

        gui.renderWindow();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
