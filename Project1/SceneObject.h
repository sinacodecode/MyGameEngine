#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

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

    Object(Model& model)
        : m_ourModel(model)
    {
        //setShaderUniforms();
        std::cout << "object made\n";
    }

    Model& getObjectModel() const { return m_ourModel; }

private:
    Model& m_ourModel;
};
#endif