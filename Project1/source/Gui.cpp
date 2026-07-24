#include <variant>

#include "include/Gui.h"

#include <GLFW/glfw3.h>

#include "include/SceneObject.h"
#include "include/InputFunctions.h"
#include "include/Scene.h"

//template<class ... Ts>
//struct overloaded :Ts ...
//{
//    using Ts::operator()...;
//};
//template<class ... Ts>
//overloaded(Ts ...) -> overloaded<Ts...>;

void attenuationGUI(Light::Attenuation& atten)
{
    ImGui::DragFloat("constant:", &atten.constant);
    ImGui::DragFloat("linear:", &atten.linear);
    ImGui::DragFloat("quadratic:", &atten.quadratic);
}

void Gui::renderScene(Renderer& renderer)
{
    ImGuiIO& m_io = ImGui::GetIO();
    m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    auto& lights = renderer.getScene()->getSceneLights();
    if (ImGui::TreeNode("Scene"))
    {
        if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_None))
        {
            //ImGui::Checkbox("backpack", )
            ImGui::Text("TBA");
            ImGui::Text("Window Pos: (%g, %g)", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            ImGui::Checkbox("Backpack", &Rendering::isBackpackRendered);
        }
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_None))
        {
            auto& lights = renderer.getScene()->getSceneLights();

            for (size_t i = 0; i < lights.size(); i++)
            {
                ImGui::PushID(static_cast<int>(i));

                // Use a value capture [i] and accept the specific light reference as the parameter
                std::visit(overloaded
                    {
                    [i](Light::PointLight& pl) // Explicitly name 'pl' to avoid scope shadow
                        {
                            ImGui::Text("Point Light #%d", static_cast<int>(i + 1));
                            ImGui::DragFloat3("Position:", &pl.position.x);

                            ImGui::ColorEdit3("Ambient Light Color", &pl.color.ambient.x);
                            ImGui::ColorEdit3("diffuse Light Color", &pl.color.diffuse.x);
                            ImGui::ColorEdit3("specular Light Color", &pl.color.specular.x);
                            attenuationGUI(pl.attenuation);
                        },
                    [i](Light::DirectionalLight& dl)
                        {
                            ImGui::Text("Directional Light #%d", static_cast<int>(i + 1));
                            ImGui::DragFloat3("direction:", &dl.direction.x);
                            ImGui::ColorEdit3("Ambient Light Color", &dl.color.ambient.x);
                            ImGui::ColorEdit3("diffuse Light Color", &dl.color.diffuse.x);
                            ImGui::ColorEdit3("specular Light Color", &dl.color.specular.x);
                        },
                    [i](Light::SpotLight& sl)
                        {
                            ImGui::Text("Spot Light #%d", static_cast<int>(i + 1));
                            ImGui::DragFloat3("Position:", &sl.position.x);
                            ImGui::DragFloat3("Direction:", &sl.direction.x);

                            ImGui::ColorEdit3("Ambient Light Color:", &sl.color.ambient.x);
                            ImGui::ColorEdit3("diffuse Light Color:", &sl.color.diffuse.x);
                            ImGui::ColorEdit3("specular Light Color:", &sl.color.specular.x);

                            ImGui::DragFloat("Cut off:", &sl.cutOff);
                            ImGui::DragFloat("outer cut off:", &sl.outerCutOff);

                            attenuationGUI(sl.attenuation);
                        },
                    }, *lights[i]);

                ImGui::Separator();
                ImGui::PopID();
            }
        }
        ImGui::TreePop();
    }

    ImGui::Text("mouse Pos: (%g, %g)", ImGui::GetMousePos().x, ImGui::GetMousePos().y);

    ImGui::Render();

    renderer.render();
}

//void Gui::drawObject(Object& object, Camera& camera, Light& light)
//{
//
//    ImGuiIO& m_io = ImGui::GetIO();
//    m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//
//    if (ImGui::TreeNode("Scene"))
//    {
//        if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_None))
//        {
//            ImGui::Text("TBA");
//            ImGui::Text("Window Pos: (%g, %g)", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
//        }
//        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_None))
//        {
//            ImGui::InputFloat("light Location x: ", &light.getPosition().x, 0.0f, 0.0f, "%.1f");
//            ImGui::InputFloat("light Location y: ", &light.getPosition().y, 0.0f, 0.0f, "%.1f");
//            ImGui::InputFloat("light Location z: ", &light.getPosition().z, 0.0f, 0.0f, "%.1f");
//            ImGui::ColorEdit3("Diffuse Color: ", &light.getDiffuseRef().x);
//            ImGui::ColorEdit3("Ambient Color: ", &light.getAmbientRef().x);
//            ImGui::ColorEdit3("Specular Color: ", &light.getSpecularRef().x);
//            ImGui::InputFloat("linear: ", &light.getLightAttenuation().m_linear, 0.0f, 0.0f, "%.1f");
//            ImGui::InputFloat("constant: ", &light.getLightAttenuation().m_constant, 0.0f, 0.0f, "%.1f");
//            ImGui::InputFloat("quadratic: ", &light.getLightAttenuation().m_quadratic, 0.0f, 0.0f, "%.1f");
//            ImGui::InputFloat("specular: ", &light.getSpecularity(), 0.0f, 0.0f, "%.1f");
//        }
//        ImGui::TreePop();
//    }
//
//    ImGui::Text("mouse Pos: (%g, %g)", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
//
//    ImGui::Render();
//}

//#include "InputFunctions.h"
void Gui::newWindow()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
}

void Gui::renderWindow()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}