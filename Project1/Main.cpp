

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



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
//const unsigned int SCR_WIDTH = 1280;
//const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float rotator{};
glm::vec3 lightPos{ 2.0f, 2.0f, 1.5f };
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

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
 
    // build and compile shaders and load model
    // -------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    Shader ourShader{ "modelLoader.vs", "modelLoader.fs" };
    Model ourModel{ std::string("C://Users/Administrator/source/repos/MyGameEngine1/Project1/Backpack/Backpack.obj") };
    Light::Attenuation atten{ 1.0f,1.0f,1.0f };
    Light lig{ lightPos, { 1.0f,1.0f,1.0f } };
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Object backpack2{ ourShader, ourModel, glm::vec3(0.0, 0.0, 0.0), camera, lig};
    float x{};
    float y{};
    float z{};
    float degree{};
    float rx{ 1 };
    float ry{};
    float rz{};
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        ImGui::InputFloat("Location x: ", &x, 0.0f, 0.0f, "%.1f");
        ImGui::InputFloat("Location y: ", &y, 0.0f, 0.0f, "%.1f");
        ImGui::InputFloat("Location z: ", &z, 0.0f, 0.0f, "%.1f");
        backpack2.drawAt(camera, glm::vec3(x, y, z), lig);
        //backpack2.draw(camera, lig);
        ImGui::InputFloat("rotation x: ", &rx, 0.0f, 0.0f, "%.1f");
        ImGui::InputFloat("rotation y: ", &ry, 0.0f, 0.0f, "%.1f");
        ImGui::InputFloat("rotation z: ", &rz, 0.0f, 0.0f, "%.1f");

        ImGui::InputFloat("degree: ", &degree, 0.0f, 0.0f, "%.1f");
        backpack2.rotate(degree, glm::vec3(rx, ry, rz));

        ImGui::InputFloat("light Location x: ", &lig.getPosition().x, 0.0f, 0.0f, "%.1f");
        ImGui::InputFloat("light Location y: ", &lig.getPosition().y, 0.0f, 0.0f, "%.1f");
        ImGui::InputFloat("light Location z: ", &lig.getPosition().z, 0.0f, 0.0f, "%.1f");
        ImGui::InputFloat("linear: ", &atten.m_linear, 0.0f, 0.0f, "%.1f");
        ImGui::InputFloat("constant: ", &atten.m_constant, 0.0f, 0.0f, "%.1f");
        ImGui::InputFloat("quadratic: ", &atten.m_quadratic, 0.0f, 0.0f, "%.1f");

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rotator+=0.01f;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lightPos+=glm::vec3(0.0f, 0.1f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lightPos += glm::vec3(0.0f, -0.1f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lightPos += glm::vec3(0.1f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lightPos += glm::vec3(-0.1f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        lightPos += glm::vec3(0.0f, 0.0f, 0.1f);
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        lightPos += glm::vec3(0.0f, 0.0f, -0.1f);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}