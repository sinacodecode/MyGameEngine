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
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

class Object {

public:

    Object(Shader& shader, Model& model, const glm::vec3& location, Camera& camera, Light light)
        : m_ourShader(shader)
        , m_ourModel(model)
        , m_location(location)
        , m_camera (camera)
        , m_light (light)
    {
        //setShaderUniforms();
        std::cout << "object made\n";
    }

    void draw(Camera& camera, const Light& light);

    void drawAt(Camera& camera, glm::vec3 loc, const Light& light);

    void rotate(float degrees, glm::vec3 axis);

private:
    Model& m_ourModel;
    Shader& m_ourShader;

    std::pair<float, glm::vec3> m_rotation{ 0.0f, glm::vec3(1.0f, 1.0f, 1.0f)};
    glm::vec3 m_location{};
    Camera& m_camera;
    Light::Attenuation m_attenuation{1.0f, 0.09f, 0.032f};
    Light& m_light;

};
#endif