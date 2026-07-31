#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <utility>
#include <memory>
#include "shader_s.h"
#include "Model.h"

#include "camera.h"
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>

#include "ConfigValues.h"

class Object {

public:

    Object() = default;

    //Object(Object&) = delete;
    //Object& operator =(Object&) = delete;

    Object(std::unique_ptr<Model> model)
        : m_ourModel(std::move(model))
    {
        //setShaderUniforms();
        std::cout << "object made\n";
    }

    const Model& getObjectModel() const
    { 
        if (!m_ourModel)
            throw std::runtime_error("Model Is Empty");
            
        return *m_ourModel;
    }

    glm::vec3 m_pos = glm::vec3{ 0.0f ,0.0f , 0.0f };
    glm::vec3 m_rotationAxis = glm::vec3{ 0.0f ,0.0f , 0.0f };
    float m_rotation = 0.0f;
private:
    std::unique_ptr<Model> m_ourModel;
};
#endif