#include <Renderer/shader_s.h>
#include <string>
//#include <variant>

std::string compileShaderWithMacros(const Light::LightVariant& light, const std::string& rawSourceCode) {
    std::string macroInjection = "#version 330 core\n"; // Ensure version is first!

    // Inspect the std::variant type at compile/init time to determine the macro
    std::visit(overloaded{
        [&](const Light::PointLight&) { macroInjection += "#define HAS_POINTLIGHT\n"; },
        [&](const Light::DirectionalLight&) { macroInjection += "#define HAS_DIRLIGHT\n"; },
        [&](const Light::SpotLight&) { macroInjection += "#define HAS_SPOTLIGHT\n"; }
        }, light);

    // Strip out any original "#version" line from rawSourceCode, then combine them
    return macroInjection + rawSourceCode;
}

void Shader::setLight(const std::string& name, const Light::LightVariant& light) const
{
    //compileShaderWithMacros(light, fragmentCode);
    std::visit(overloaded
        {
        [&](const Light::PointLight& l)
        {
            setBool("hasPointLights", true);
            setVec3(name + ".position", l.position);

            setVec3(name + ".ambient", l.color.ambient);
            setVec3(name + ".diffuse", l.color.diffuse);
            setVec3(name + ".specular", l.color.specular);

            setAttenuation(name, l.attenuation);
        },
        [&](const Light::DirectionalLight& l)
        {
            setBool("hasDirLight", true);
            setVec3(name + ".direction", l.direction);

            setVec3(name + ".ambient", l.color.ambient);
            setVec3(name + ".diffuse", l.color.diffuse);
            setVec3(name + ".specular", l.color.specular);
        },
        [&](const Light::SpotLight& l)
        {
            setBool("hasSpotLight", true);
            setVec3(name + ".position", l.position);
            setVec3(name + ".direction", l.direction);

            setFloat(name + ".cutOff", glm::cos(glm::radians(l.cutOff)));
            setFloat(name + ".outerCutOff", glm::cos(glm::radians(l.outerCutOff)));

            setVec3(name + ".ambient", l.color.ambient);
            setVec3(name + ".diffuse", l.color.diffuse);
            setVec3(name + ".specular", l.color.specular);

            setAttenuation(name, l.attenuation);
        }
        }, light);
}
