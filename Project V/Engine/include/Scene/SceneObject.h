#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <utility>
#include <memory>
#include <Renderer/shader_s.h>
#include <Renderer/Model.h>

#include <Renderer/camera.h>
#include <Renderer/Light.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Core/ConfigValues.h>

class Object {

public:

    Object() = default;

    Object(std::unique_ptr<Model> model, int id = 0)
        : m_ID{id}, m_ourModel(std::move(model))
    {
    }

    [[nodiscard]] const Model& getObjectModel() const
    { 
        if (!m_ourModel)
            throw std::runtime_error("Model Is Empty");
            
        return *m_ourModel;
    }

    glm::vec3 m_pos = glm::vec3{ 0.0F ,0.0F , 0.0F };
    glm::vec3 m_rotationAxis = glm::vec3{ 0.0F ,0.0F , 0.0F };
    float m_rotation = 0.0F;
    int m_ID;
private:
    std::unique_ptr<Model> m_ourModel;
};
#endif