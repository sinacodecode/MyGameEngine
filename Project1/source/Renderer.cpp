#include "include/Renderer.h"
#include <variant>
//NEEDS REFACTORING !!!

void Renderer::render()
{
    
    m_shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(m_scene.getSceneCamera().Zoom), (float)Rendering::SCR_WIDTH / (float)Rendering::SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = m_scene.getSceneCamera().GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down

    m_shader.setMat4("projection", projection);
    m_shader.setMat4("view", view);
    m_shader.setMat4("model", model);

    //m_ourShader.setVec3("diffuseLightColor", light.getDiffuseColor());
    //m_ourShader.setVec3("ambientLightColor", light.getAmbientColor());

    //needs more refactoring
    for (auto& l : m_scene.getSceneLights())
    {
        std::visit(overloaded
            {
                [&](Light::PointLight light)
                {
                    for(int i=0; i<m_scene.m_pointLightCount;i++)
                        m_shader.setLight("pointLights[" + std::to_string(i) + "]", light);
                    std::cout << "void Renderer::render() FROM RENDERER!\t" << light.position.x << '\n';
                },
                [&](Light::DirectionalLight light)
                {
                    m_shader.setLight("dirLight", light);
                    std::cout << "CALLED m_shader.setLight(\"dirLight\", light); FROM RENDERER!\n";
                },
                [&](Light::SpotLight light)
                {
                    m_shader.setLight("spotLight", light);
                    std::cout << "CALLED m_shader.setLight(\"spotLight\", light); FROM RENDERER!\n";
                }
            }, l);
    }
    m_shader.setLight("spotLight", m_scene.getSceneLights()[0]);
    m_shader.setVec3("viewPos", m_scene.getSceneCamera().Position);
    //m_ourShader.setVec3("lightSpecular", glm::vec3(1.0f, 1.0f, 1.0f));

    //int i{ 0 };
    //for (i = 0; i < 2; i++) {
    //m_scene.getSceneObjects()[i].getObjectModel().Draw(m_shader);
    //}

    for (auto& object : m_scene.getSceneObjects())
        object.getObjectModel().Draw(m_shader);
}
