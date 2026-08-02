#include "include/Renderer.h"
#include "include/Transformations.h"
#include <variant>
//NEEDS REFACTORING !!!

void Renderer::render()
{
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    m_shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(m_scene->getSceneCamera().Zoom), (float)Rendering::SCR_WIDTH / (float)Rendering::SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = m_scene->getSceneCamera().GetViewMatrix();

    for (auto& object : m_scene->getSceneObjects())
    {
        
        glm::mat4 model = glm::mat4(1.0f);
        Transformations::translate(model, object->m_pos);
		Transformations::rotate(model, object->m_rotation, object->m_rotationAxis);
        Transformations::scale(model, glm::vec3(1.0f));
        
        if (object == m_scene->getSceneObjects().back())
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
        else
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
        //if(m_scene->getSceneObjects().back()==object)
        //{
        //    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //    glStencilMask(0x00);
        //    glDisable(GL_DEPTH_TEST);
        //    outlineShader->use();

        //    glm::mat4 outlineModel = model;
        //    Transformations::scale(outlineModel, glm::vec3(Shaders::outlineScale * 1.0f));
        //    outlineShader->setMat4("projection", projection);
        //    outlineShader->setMat4("view", view);
        //    outlineShader->setMat4("model", outlineModel);
        //    object->getObjectModel().Draw(*outlineShader);

        //    
        //    glStencilMask(0xFF);
        //    glStencilFunc(GL_ALWAYS, 0, 0xFF);
        //    glEnable(GL_DEPTH_TEST);
        //}


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
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    outlineShader->use();

    glm::mat4 outlineModel(1.0f);
    Transformations::translate(outlineModel, m_scene->getSceneObjects().back()->m_pos);
    Transformations::rotate(outlineModel, m_scene->getSceneObjects().back()->m_rotation, m_scene->getSceneObjects().back()->m_rotationAxis);
    Transformations::scale(outlineModel, glm::vec3(Shaders::outlineScale));
    
    outlineShader->setMat4("projection", projection);
    outlineShader->setMat4("view", view);
    outlineShader->setMat4("model", outlineModel);

    m_scene->getSceneObjects().back()->getObjectModel().Draw(*outlineShader);
    //HOW DO I ACCESS THE LAST OBJECT IF IT IS OUT OF SCOPE?
    glStencilMask(0xFF);

    //already added in the beginning
    //glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);

}
