#include "include/Renderer.h"
#include "include/Transformations.h"
#include <variant>
//NEEDS REFACTORING !!!

void Renderer::render()
{

    m_shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(m_scene->getSceneCamera().Zoom), (float)Rendering::SCR_WIDTH / (float)Rendering::SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = m_scene->getSceneCamera().GetViewMatrix();

    for (auto& object : m_scene->getSceneObjects())
    {
        glm::mat4 model = glm::mat4(1.0f);
        Transformations::translate(model, object->m_pos);
		Transformations::rotate(model, object->m_rotation, object->m_rotationAxis);
        Transformations::scale(model, glm::vec3(1.0f));

		std::cout << model[0][0] << " " << model[0][1] << " " << model[0][2] << " " << model[0][3] << "\n";
        std::cout << model[1][0] << " " << model[1][1] << " " << model[1][2] << " " << model[1][3] << "\n";
        std::cout << model[2][0] << " " << model[2][1] << " " << model[2][2] << " " << model[2][3] << "\n";
        std::cout << model[3][0] << " " << model[3][1] << " " << model[3][2] << " " << model[3][3] << "\n";
        std::cout << "-----------------------------------------------------------------------------" << "\n";

        m_shader.setMat4("projection", projection);
        m_shader.setMat4("view", view);
        m_shader.setMat4("model", model);

        int pointLightIndex{ 0 };
        for (auto& l : m_scene->getSceneLights())
        {
            std::visit(overloaded
                {
                    [&](Light::PointLight light)
                    {

                        m_shader.setLight("pointLights[" + std::to_string(pointLightIndex) + "]", light);
                        ++pointLightIndex;
                    },
                    [&](Light::DirectionalLight light)
                    {
                        m_shader.setLight("dirLight", light);
                    },
                    [&](Light::SpotLight light)
                    {
                        m_shader.setLight("spotLight", light);
                    }
                }, *l);
        }
		
        m_shader.setInt("numberOfPointLights", pointLightIndex);
        m_shader.setVec3("viewPos", m_scene->getSceneCamera().Position);
        object->getObjectModel().Draw(m_shader);
    }
}
