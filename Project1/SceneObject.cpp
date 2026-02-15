#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "SceneObject.h"

void Object::draw(const Camera& camera, const Light& light)
{
    m_ourShader.use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Rendering::SCR_WIDTH / (float)Rendering::SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_location); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down

    m_ourShader.setMat4("projection", projection);
    m_ourShader.setMat4("view", view);
    m_ourShader.setMat4("model", model);

    m_ourShader.setVec3("diffuseLightColor", light.getDiffuseColor());
    m_ourShader.setVec3("ambientLightColor", light.getAmbientColor());

    m_ourShader.setLight("light", light);
    m_ourShader.setVec3("viewPos", camera.Position);
    //m_ourShader.setVec3("lightSpecular", glm::vec3(1.0f, 1.0f, 1.0f));

    m_ourModel.Draw(m_ourShader);

}

void Object::drawAt(const Camera& camera, glm::vec3 loc,const Light& light)
{
    m_ourShader.use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Rendering::SCR_WIDTH / (float)Rendering::SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 move{0.0f, glm::sin(glfwGetTime()*2), 0.0f};

    model = glm::translate(model, loc); // translate it down so it's at the center of the scene
    model = glm::rotate(model, static_cast<float>(glm::radians(m_rotation.first*glfwGetTime())), m_rotation.second);
    model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down

    m_ourShader.setMat4("projection", projection);
    m_ourShader.setMat4("view", view);
    m_ourShader.setMat4("model", model);

    m_ourShader.setVec3("diffuseLightColor", 1.7f, 1.5f, 1.3f);
    m_ourShader.setVec3("ambientLightColor", 0.3f, 0.5f, 1.0f);

    m_ourShader.setLight("light", light);
    m_ourShader.setVec3("viewPos", camera.Position);
    //m_ourShader.setVec3("lightSpecular", glm::vec3(1.0f, 1.0f, 1.0f));

    m_ourModel.Draw(m_ourShader);

}

void Object::rotate(float degrees, glm::vec3 axis)
{
    this->m_rotation.first = degrees;
    this->m_rotation.second = axis;
}