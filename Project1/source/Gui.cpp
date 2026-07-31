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

	static const char* lightTypes[] = { "Point Light", "Directional Light", "Spot Light" };
    static int selectedLight = 0;
    if (ImGui::TreeNode("Scene"))
    {
        if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_None))
        {
            //ImGui::Checkbox("backpack", )
            ImGui::Text("TBA");
            ImGui::Text("Window Pos: (%g, %g)", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            if (ImGui::Button("AddBackpack"))
            {
                renderer.getScene()->pushObject(std::make_unique<Object>(
                    std::make_unique<Model>("../Resources/Models/Backpack/Backpack.obj")
                ));
            }
            ImGui::Text("Window Pos: (%g)", renderer.getScene()->getSceneObjects().back()->m_pos.x);
            ImGui::DragFloat3("Position:", &renderer.getScene()->getSceneObjects().back()->m_pos.x);
            ImGui::DragFloat3("rotation Axis:", &renderer.getScene()->getSceneObjects().back()->m_rotationAxis.x, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Rotation", &renderer.getScene()->getSceneObjects().back()->m_rotation, 1.0f, -360.0f, 360.0f);
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

                ImGui::SameLine();
                if (ImGui::Button("Remove"))
                {
                    // Remove the light from the Scene. Adjust loop index to account for element shift.
                    renderer.getScene()->removeLightAt(i);
                    ImGui::PopID();
                    if (i != 0) --i; // step back so next iteration won't skip an element
                    continue;
                }

                ImGui::Separator();
                ImGui::PopID();
            }
            if (ImGui::Combo("Add Lights", &selectedLight, lightTypes, IM_ARRAYSIZE(lightTypes)))
            {
                // Code here runs instantly when a new item is clicked
                printf("Selected item changed to: %s\n", lightTypes[selectedLight]);
                switch (selectedLight)
                {
                    case 0:
                        renderer.getScene()->pushLight(std::make_unique<Light::LightVariant>(Light::PointLight{}));
						break;
                    case 1:
                        renderer.getScene()->pushLight(std::make_unique<Light::LightVariant>(Light::DirectionalLight{}));
                        break;
                    case 2:
                        renderer.getScene()->pushLight(std::make_unique<Light::LightVariant>(Light::SpotLight{}));
                        break;
                }
            }
        }
        ImGui::TreePop();
    }

    ImGui::Text("mouse Pos: (%g, %g)", ImGui::GetMousePos().x, ImGui::GetMousePos().y);

    //renderer.render();
}

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