#include "Renderer.h"

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

    m_shader.setLight("light", m_scene.getSceneLights()[0]);
    m_shader.setVec3("viewPos", m_scene.getSceneCamera().Position);
    //m_ourShader.setVec3("lightSpecular", glm::vec3(1.0f, 1.0f, 1.0f));

    m_scene.getSceneObjects()[0].getObjectModel().Draw(m_shader);
}
