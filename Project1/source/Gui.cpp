#include <variant>
#include <charconv>

#include "include/Gui.h"

#include <GLFW/glfw3.h>

#include "include/SceneObject.h"
#include "include/InputFunctions.h"
#include "include/Scene.h"

void attenuationGUI(Light::Attenuation& atten)
{
    ImGui::DragFloat("constant:", &atten.constant);
    ImGui::DragFloat("linear:", &atten.linear);
    ImGui::DragFloat("quadratic:", &atten.quadratic);
}

void Gui::renderScene(Renderer& renderer)
{
    ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400.0F, 600.0F), ImGuiCond_FirstUseEver);

    ImGuiIO& m_io = ImGui::GetIO();
    m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    auto& lights = renderer.getScene()->getSceneLights();

    if (ImGui::TreeNode("Scene"))
    {
        objectsMenu(renderer);
        lightsMenu(renderer);
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

void Gui::objectsMenu(Renderer& renderer)
{

    static int item_selected_idx = 0;
    static bool item_highlight = false;
    static const char* modelTypes[32]{};
    int item_highlighted_idx = -1;

    static char stringBuffer[32 * 16];
    char* currentBufferPos = stringBuffer;
    std::size_t i = 0;
    for (const auto& ID : renderer.getScene()->getSceneObjects())
    {
        if (i >= 32) break; // Prevent array overflow

        // 2. Call std::to_chars by passing the buffer bounds
        auto [ptr, ec] = std::to_chars(currentBufferPos, currentBufferPos + 15, ID->m_ID);

        if (ec == std::errc{}) {
            *ptr = '\0'; // 3. Manually null-terminate for const char* compatibility
            modelTypes[i] = currentBufferPos;
            currentBufferPos = ptr + 1; // Move pointer past the null terminator
        }
        i++;
    }

    if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_None))
    {
        //ImGui::Checkbox("backpack", )
        ImGui::Text("TBA");
        ImGui::Text("Window Pos: (%g, %g)", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);

        auto& objects = renderer.getScene()->getSceneObjects();

        ImGui::Text("Full-width:");
        if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
        {
            for (int n = 0; n < objects.size(); n++)
            {
                ImGui::PushID(n);
                bool is_selected = (item_selected_idx == n);
                ImGuiSelectableFlags flags = (item_highlighted_idx == n) ? ImGuiSelectableFlags_Highlight : 0;
                if (ImGui::Selectable(modelTypes[n], is_selected, flags))
                    item_selected_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                    Rendering::selectedObject = item_selected_idx;
                }

                ImGui::Separator();
                ImGui::PopID();
            }
            ImGui::EndListBox();
        }
        
        for (size_t j = 0; j < objects.size(); j++)
        {
            ImGui::PushID(static_cast<int>(j));

            ImGui::Text("Model ID: (%g)", objects[j]->m_ID);
            ImGui::DragFloat3("Position:##03", &objects[j]->m_pos.x);
            ImGui::DragFloat3("rotation Axis:##03", &objects[j]->m_rotationAxis.x, 0.01F, 0.0F, 1.0F);
            ImGui::DragFloat("Rotation##03", &objects[j]->m_rotation, 1.0F, -360.0F, 360.0F);

            ImGui::Separator();

            ImGui::Separator();
            ImGui::PopID();
        }
        if (ImGui::Button("AddBackpack"))
        {
            renderer.getScene()->pushObject(std::make_unique<Object>(std::make_unique<Model>("../Resources/Models/Backpack/Backpack.obj", 0),0));
        }

        if (ImGui::Button("AddGrass"))
        {
            renderer.getScene()->pushObject(std::make_unique<Object>(std::make_unique<Model>("../Resources/Models/Grass/Grass.obj", 0), 1));
        }

        if (ImGui::Button("AddGlassBottle"))
        {
            renderer.getScene()->pushObject(std::make_unique<Object>(std::make_unique<Model>("../Resources/Models/GlassBottle/GlassBottle.obj", 2), 2));
        }
    }
}

void Gui::lightsMenu(Renderer& renderer)
{

    static int selectedLight = 0;
    static const char* lightTypes[] = { "Point Light", "Directional Light", "Spot Light" };

    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_None))
    {
        auto& lights = renderer.getScene()->getSceneLights();
        for (size_t i = 0; i < lights.size(); i++)
        {
            ImGui::PushID(static_cast<int>(i));
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

            bool remove = ImGui::Button("Remove");

            ImGui::Separator();

            if (remove)
            {
                renderer.getScene()->removeLightAt(i);
            }
            ImGui::PopID();
        }
        if (ImGui::Combo("Add Lights", &selectedLight, lightTypes, IM_ARRAYSIZE(lightTypes)))
        {

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