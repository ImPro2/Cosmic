module;
#include "cspch.hpp"
#include <entt/entt.hpp>
#include <IconsFontAwesome6.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
module Editor.Panels.InspectorPanel;

CS_MODULE_LOG_INFO(Editor, Panels.InspectorPanel);

import Cosmic.Script.ScriptEngine;

namespace Cosmic
{

    InspectorPanel::InspectorPanel()
        : Panel("Inspector Panel")
    {
    }

    void InspectorPanel::OnImGuiRender()
    {
        if (!mOpen)
            return;

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
            ImGui::PushID(reinterpret_cast<void*>(typeid(Component).hash_code()));
            ImGuiIO& io = ImGui::GetIO();
            auto& component = entity.GetComponent<Component>();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
            float32 lineHeight = io.FontDefault->FontSize + 8.0f;

            const bool open = ImGui::TreeNodeEx("dummyID", treeNodeFlags, name.c_str());

            if (open)
                ImGui::SameLine(ImGui::GetContentRegionAvail().x + 0.34f * lineHeight);
            else
                ImGui::SameLine(ImGui::GetContentRegionAvail().x - 0.5f * lineHeight);

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

            ImGui::PopID();
        }
    }

    template<typename Component>
    static void DisplayAddComponent(const char* name, Entity entity)
    {
        if (ImGui::MenuItem(name))
        {
            if (!entity.HasComponent<Component>())
            {
                entity.AddComponent<Component>();
                CS_LOG_INFO("Added {} to {}", name, entity.GetComponent<TagComponent>().Tag.c_str());
            }
            else
                CS_LOG_WARN("This entity already has a {}.", name);
            
            ImGui::CloseCurrentPopup();
        }
    }

    static void DrawVec3(const char* label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        //ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();

        float32 lineHeight = io.FontDefault->FontSize + 2.0f * style.FramePadding.y;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::PushItemWidth(columnWidth);
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));

        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::PushItemWidth(columnWidth);
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));

        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::PushItemWidth(columnWidth);
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar(2);
        ImGui::Columns(1);
        ImGui::PopID();
    }

    void InspectorPanel::RenderComponents(Entity entity)
    {
        // Tag

        ImGuiStyle& style = ImGui::GetStyle();

        const char* addComponentButtonLabel = "+ Add Component";
        ImVec2 labelSize = ImGui::CalcTextSize(addComponentButtonLabel, NULL, true);
        ImVec2 buttonSize = { labelSize.x + style.FramePadding.x, labelSize.y + 2.0f * style.FramePadding.y };

        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - buttonSize.x - 10.0f);
        auto& tag = entity.GetComponent<TagComponent>().Tag;
        char tagBuffer[256];
        memset(tagBuffer, 0, sizeof(tagBuffer));
        std::strncpy(tagBuffer, tag.c_str(), sizeof(tagBuffer));
        
        if (ImGui::InputText("##Tag", tagBuffer, sizeof(tagBuffer)))
            tag = String(tagBuffer);

        ImGui::PopItemWidth();
        
        // Add components

        ImGui::SameLine(ImGui::GetContentRegionAvail().x - buttonSize.x);
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("+ Add Component"))
            ImGui::OpenPopup("Add Component");

        if (ImGui::BeginPopup("Add Component"))
        {
            if (ImGui::BeginMenu("Rendering"))
            {
                DisplayAddComponent<CameraComponent>("Camera Component", entity);
                DisplayAddComponent<SpriteRendererComponent>("Sprite Renderer Component", entity);

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Scripting"))
            {
                DisplayAddComponent<NativeScriptComponent>("Native Script Component", entity);

                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();

        // Components

        RenderComponent<TransformComponent>("Transform Component", entity, [](TransformComponent& component)
        {
            DrawVec3("Translation", component.Translation);
            DrawVec3("Rotation", component.Rotation);
            DrawVec3("Scale", component.Scale, 1.0f);
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
        RenderComponent<NativeScriptComponent>("Native Script Component", entity, [](NativeScriptComponent& component)
        {
            auto callback = [](ImGuiInputTextCallbackData* data) -> int32
            {
                if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
                {
                    String* userData = (String*)data->UserData;
                    userData->resize(data->BufSize);
                    data->Buf = (char*)userData->c_str();
                }
                return 0;
            };

            auto& className = component.ClassName;
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
            ImGui::InputText("##InputScript", (char*)className.c_str(), className.size(), ImGuiInputTextFlags_CallbackResize, callback, (void*)&className);
            ImGui::SameLine();
            
            if (ImGui::Button("Load"))
            {
                component.Bind(className);
            }
            ImGui::SameLine();
            if (ImGui::Button("Create"))
            {

            }
        });
    }

}