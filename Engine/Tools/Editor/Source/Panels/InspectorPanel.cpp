module;
#include "cspch.hpp"
#include <entt/entt.hpp>
#include <IconsFontAwesome6.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
module Editor.Panels.InspectorPanel;

CS_MODULE_LOG_INFO(Editor, Panels.InspectorPanel);

namespace Cosmic
{

    InspectorPanel::InspectorPanel()
        : Panel("Inspector Panel")
    {
    }

    void InspectorPanel::OnImGuiRender()
    {
        if (ImGui::Begin("Inspector", &mOpen))
        {
            SceneHierarchyPanel* panel = ModuleSystem::Get<SceneHierarchyPanel>();
            Entity selectedEntity = panel->GetSelectedEntity();

            if (selectedEntity)
                RenderComponents(selectedEntity);
            else
                ImGui::Text("No entity selected.");

            ImGui::End();
        }
    }

    template<typename Component, typename Callback>
    static void RenderComponent(const String& name, Entity entity, Callback callback)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        if (entity.HasComponent<Component>())
        {
            ImGuiIO& io = ImGui::GetIO();
            auto& component = entity.GetComponent<Component>();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
            float32 lineHeight = io.FontDefault->FontSize + 8.0f;

            const bool open = ImGui::TreeNodeEx("dummyID", treeNodeFlags, name.c_str());

            ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight * 0.5f);
            if (ImGui::Button(ICON_FA_GEAR, ImVec2{ lineHeight, lineHeight }))
            {
                ImGui::OpenPopup("Component Settings");
            }

            bool removeComponent = false;

            ImGui::PopStyleVar();

            if (ImGui::BeginPopup("Component Settings"))
            {
                if (ImGui::MenuItem("Reset"))
                    component.Reset();
                if (ImGui::MenuItem("RemoveComponent"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if (open)
            {
                callback(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                entity.RemoveComponent<Component>();
        }
    }

    void InspectorPanel::RenderComponents(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;
        ImGui::InputText("Entity Tag", (char*)tag.c_str(), tag.size());

        RenderComponent<TransformComponent>("Transform Component", entity, [](TransformComponent& component)
        {
            auto& transform = component.Transform;

            ImGui::DragFloat3("Translation", glm::value_ptr(transform[3]), 0.1f);
        });
        RenderComponent<SpriteRendererComponent>("Sprite Renderer Component", entity, [](SpriteRendererComponent& component)
        {
            ImGui::ColorEdit4("Color", &component.Color.x);
        });
        RenderComponent<CameraComponent>("Camera Component", entity, [](CameraComponent& component)
        {
            auto& camera = component.Camera;

            ImGui::Checkbox("Primary", &component.Primary);
            ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);

            const char* projectionTypeStrings[]     = { "Perspective", "Orthographic" };
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

            if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                    {
                        currentProjectionTypeString = projectionTypeStrings[i];
                        camera.SetProjectionType((EProjectionType)i);
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            if (camera.GetProjectionType() == EProjectionType::Perspective)
            {
                float32 fov  = glm::degrees(camera.GetPerspectiveVerticalFOV());
                float32 near = camera.GetPerspectiveNearClip();
                float32 far  = camera.GetPerspectiveFarClip();

                if (ImGui::DragFloat("Vertical FOV", &fov))
                    camera.SetPerspectiveVerticalFOV(glm::radians(fov));
                if (ImGui::DragFloat("Near Clip", &near))
                    camera.SetPerspectiveNearClip(near);
                if (ImGui::DragFloat("Far Clip", &far))
                    camera.SetPerspectiveFarClip(far);
            }
            else if (camera.GetProjectionType() == EProjectionType::Orthographic)
            {
                float32 size = camera.GetOrthographicSize();
                float32 near = camera.GetOrthographicNearClip();
                float32 far  = camera.GetOrthographicFarClip();

                if (ImGui::DragFloat("Size", &size))
                    camera.SetOrthographicSize(size);
                if (ImGui::DragFloat("Near Clip", &near))
                    camera.SetOrthographicNearClip(near);
                if (ImGui::DragFloat("Far Clip", &far))
                    camera.SetOrthographicFarClip(far);
            }
        });

        // Add component

        if (ImGui::Button("+ Add Component"))
            ImGui::OpenPopup("Add Component");

        if (ImGui::BeginPopup("Add Component"))
        {
            if (ImGui::BeginMenu("Rendering"))
            {
                if (ImGui::MenuItem("Camera Component"))
                {
                    if (!entity.HasComponent<CameraComponent>())
                    {
                        entity.AddComponent<CameraComponent>();
                        CS_LOG_INFO("Added Camera Component to {}", tag.c_str());
                    }
                    else
                        CS_LOG_WARN("This entity already has a camera component.");
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("Sprite Renderer Component"))
                {
                    if (!entity.HasComponent<SpriteRendererComponent>())
                    {
                        entity.AddComponent<SpriteRendererComponent>();
                        CS_LOG_INFO("Added Sprite Renderer Component to {}", tag.c_str());
                    }
                    else
                        CS_LOG_WARN("This entity already has a sprite renderer component.");
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }
    }

}