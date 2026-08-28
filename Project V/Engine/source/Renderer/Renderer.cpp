#include <Renderer/Renderer.h>
#include <Renderer/Transformations.h>
#include <variant>
#include <map>
//NEEDS REFACTORING !!!

void Renderer::render()
{
    projection = glm::perspective(glm::radians(m_scene->getSceneCamera().Zoom), (float)Rendering::SCR_WIDTH / (float)Rendering::SCR_HEIGHT, 0.1f, 100.0f);
    view = m_scene->getSceneCamera().GetViewMatrix();

    clearBuffers();

    updateScene();
    renderOpaqueObjects();
    renderOutlinedObject();
    renderTransparentObjects();
}

void Renderer::updateScene()
{

    for (auto& lights : m_scene->getSceneLights())
    {
        
        std::visit(overloaded
            {
                [&](Light::PointLight& light)
                {
                    glm::mat4 lightMatrix = glm::mat4(1.0f);
                    Transformations::translate(lightMatrix, light.position);
                },
                [&](Light::DirectionalLight& light)
                {
                    glm::mat4 lightMatrix = glm::mat4(1.0f);
                    Transformations::rotateEuler(lightMatrix, light.rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
                    Transformations::rotateEuler(lightMatrix, light.rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
                    Transformations::rotateEuler(lightMatrix, light.rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
                },
                [&](Light::SpotLight& light)
                {
                    glm::mat4 lightMatrix = glm::mat4(1.0f);
                    Transformations::translate(lightMatrix, light.position);
                    Transformations::rotateEuler(lightMatrix, light.rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
                    Transformations::rotateEuler(lightMatrix, light.rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
                    Transformations::rotateEuler(lightMatrix, light.rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
                }
            }, *lights);
    }
}

void Renderer::renderOpaqueObjects()
{


    for (auto& object : m_scene->getSceneObjects())
    {
        Shader& shader = *m_shaders[object->getObjectModel().m_shaderID];
        shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        Transformations::translate(model, object->m_pos);
        //Transformations::rotateEuler(model, object->m_rotation, object->m_rotationAxis);
        Transformations::rotateEuler(model, object->m_rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
        Transformations::rotateEuler(model, object->m_rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
        Transformations::rotateEuler(model, object->m_rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));


        Transformations::scale(model, glm::vec3(1.0f));

        if (object == m_scene->getSceneObjects()[Rendering::selectedObject])
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
        else
            glStencilFunc(GL_ALWAYS, 0, 0xFF);

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);

        int pointLightIndex{ 0 };
        for (auto& l : m_scene->getSceneLights())
        {
            std::visit(overloaded
                {
                    [&](Light::PointLight light)
                    {

                        shader.setLight("pointLights[" + std::to_string(pointLightIndex) + "]", light);
                        ++pointLightIndex;
                    },
                    [&](Light::DirectionalLight light)
                    {
                        shader.setLight("dirLight", light);
                    },
                    [&](Light::SpotLight light)
                    {
                        shader.setLight("spotLight", light);
                    }
                }, *l);
        }

        shader.setInt("numberOfPointLights", pointLightIndex);
        shader.setVec3("viewPos", m_scene->getSceneCamera().Position);

        object->getObjectModel().Draw(shader);
    }
}

void Renderer::renderOutlinedObject()
{
    if (Rendering::selectedObject >= m_scene->getSceneObjects().size())
        return;
    
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    
    m_shaders[1]->use();

    glm::mat4 outlineModel(1.0f);
    Transformations::translate(outlineModel, m_scene->getSceneObjects()[Rendering::selectedObject]->m_pos);
    Transformations::rotateEuler(outlineModel, m_scene->getSceneObjects()[Rendering::selectedObject]->m_rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
    Transformations::rotateEuler(outlineModel, m_scene->getSceneObjects()[Rendering::selectedObject]->m_rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
    Transformations::rotateEuler(outlineModel, m_scene->getSceneObjects()[Rendering::selectedObject]->m_rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
    Transformations::scale(outlineModel, glm::vec3(Shaders::outlineScale));

    m_shaders[1]->setMat4("projection", projection);
    m_shaders[1]->setMat4("view", view);
    m_shaders[1]->setMat4("model", outlineModel);

    m_scene->getSceneObjects()[Rendering::selectedObject]->getObjectModel().Draw(*m_shaders[1]);

    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::renderTransparentObjects()
{
	auto& transparentObjects = m_scene->getSceneTransparentObjects();
    std::sort(transparentObjects.begin(), transparentObjects.end(),
        [&](const auto& a, const auto& b)
        {
            float distanceA = glm::length(m_scene->getSceneCamera().Position - a->m_pos);
            float distanceB = glm::length(m_scene->getSceneCamera().Position - b->m_pos);
            return distanceA > distanceB;
        }
    );
    for (auto& object : transparentObjects)
    {
        Shader& shader = *m_shaders[object->getObjectModel().m_shaderID];
        shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        Transformations::translate(model, object->m_pos);
        Transformations::rotateEuler(model, object->m_rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
        Transformations::rotateEuler(model, object->m_rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
        Transformations::rotateEuler(model, object->m_rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
        Transformations::scale(model, glm::vec3(1.0f));

        if (object == m_scene->getSceneObjects()[Rendering::selectedObject])
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
        else
            glStencilFunc(GL_ALWAYS, 0, 0xFF);

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);

        int pointLightIndex{ 0 };
        for (auto& l : m_scene->getSceneLights())
        {
            std::visit(overloaded
                {
                    [&](Light::PointLight light)
                    {

                        shader.setLight("pointLights[" + std::to_string(pointLightIndex) + "]", light);
                        ++pointLightIndex;
                    },
                    [&](Light::DirectionalLight light)
                    {
                        shader.setLight("dirLight", light);
                    },
                    [&](Light::SpotLight light)
                    {
                        shader.setLight("spotLight", light);
                    }
                }, *l);
        }

        shader.setInt("numberOfPointLights", pointLightIndex);
        shader.setVec3("viewPos", m_scene->getSceneCamera().Position);
        object->getObjectModel().Draw(shader);
    }
}

void Renderer::clearBuffers()
{
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
}

