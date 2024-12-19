#include "cspch.hpp"
#include "Editor/Panels/SceneHierarchyPanel.hpp"
#include "InspectorPanel.hpp"
#include <entt/entt.hpp>
#include <IconsFontAwesome6.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "Editor/Event/ComponentEvents.hpp"
#include "UI/ImGuiUtils/DrawTypes.hpp"

CS_MODULE_LOG_INFO(Editor, Panels.InspectorPanel);

#include "Script/NativeScriptEngine.hpp"

namespace Cosmic
{

    InspectorPanel::InspectorPanel()
        : IPanel("Inspector")
    {
    }

    void InspectorPanel::OnImGuiRender()
    {
        if (!mOpen)
            return;

        if (ImGui::Begin(mPanelName.c_str(), &mOpen))
        {
            Ref<SceneHierarchyPanel> sceneHierarchyPanel = ModuleSystem::Get<SceneHierarchyPanel>();
            const Vector<Entity>& selectedEntities = sceneHierarchyPanel->GetSelectedEntities();
            
            if (selectedEntities.size() == 1)
                RenderComponents(selectedEntities[0]);
            else
                ImGui::Text("No entity selected.");
        }

		ImGui::End();
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
                Component component = entity.AddComponent<Component>();
                EventSystem::DeferEvent<ComponentAddedEvent>(entity, new Component());
            }
            else
                CS_LOG_WARN("This entity already has a {}.", name);
            
            ImGui::CloseCurrentPopup();
        }
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
			ImGuiUtils::DrawVec3("Translation", component.Translation, { 0.0f, 0.0f, 0.0f });
            ImGuiUtils::DrawVec3("Rotation",    component.Rotation,    { 0.0f, 0.0f, 0.0f });
            ImGuiUtils::DrawVec3("Scale",       component.Scale,       { 1.0f, 1.0f, 1.0f });
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

            EProjectionType projectionType = camera.GetProjectionType();

            if (ImGuiUtils::DrawEnum<EProjectionType>("Projection", projectionType, EProjectionType::Orthographic, EProjectionTypeToStr, EProjectionTypeFromStr))
            {
                camera.SetProjectionType(projectionType);
            }

            if (projectionType == EProjectionType::Perspective)
            {
                float32 fov  = glm::degrees(camera.GetPerspectiveVerticalFOV());
                float32 near = camera.GetPerspectiveNearClip();
                float32 far  = camera.GetPerspectiveFarClip();

                if (ImGuiUtils::DrawFloat("Field Of View", fov, glm::radians(45.0f)))
                    camera.SetPerspectiveVerticalFOV(fov);
                if (ImGuiUtils::DrawFloat("Near Clip", near, 0.01f))
                    camera.SetPerspectiveNearClip(near);
                if (ImGuiUtils::DrawFloat("Far Clip", far, 1000.0f))
                    camera.SetPerspectiveFarClip(far);
            }
            else if (projectionType == EProjectionType::Orthographic)
            {
                float32 size = camera.GetOrthographicSize();
                float32 near = camera.GetOrthographicNearClip();
                float32 far  = camera.GetOrthographicFarClip();

                if (ImGuiUtils::DrawFloat("Size", size, 10.0f))
                    camera.SetOrthographicSize(size);
                if (ImGuiUtils::DrawFloat("Near Clip", near, -1.0f))
                    camera.SetOrthographicNearClip(near);
                if (ImGuiUtils::DrawFloat("Far Clip", far, 1.0f))
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

            if (ImGui::Button("Bind"))
                component.ShouldLoad = true;

            // Render fields

            if (!component.Instance)
                return;

            NativeScriptRegistry& registry = NativeScriptEngine::GetRegistry();
            Vector<IField*>&      fields   = registry.GetScriptInstanceFields(component.Instance);

            for (IField* field : fields)
            {
                switch (field->GetType())
                {
					case EFieldType::Float32: ImGuiUtils::DrawFloat (field->GetName(), field->GetValue<float32>(), field->GetDefaultValue<float32>()); break;
					case EFieldType::Float2:  ImGuiUtils::DrawFloat2(field->GetName(), field->GetValue<float2>(),  field->GetDefaultValue<float2>());  break;
                    case EFieldType::Float3:  ImGuiUtils::DrawFloat3(field->GetName(), field->GetValue<float3>(),  field->GetDefaultValue<float3>());  break;
					case EFieldType::Float4:  ImGuiUtils::DrawFloat2(field->GetName(), field->GetValue<float2>(),  field->GetDefaultValue<float2>());  break;
					case EFieldType::Int32:   ImGuiUtils::DrawInt   (field->GetName(), field->GetValue<int32>(),   field->GetDefaultValue<int32>());   break;
					case EFieldType::Int2:    ImGuiUtils::DrawInt2  (field->GetName(), field->GetValue<int2>(),    field->GetDefaultValue<int2>());    break;
					case EFieldType::Int3:    ImGuiUtils::DrawInt3  (field->GetName(), field->GetValue<int3>(),    field->GetDefaultValue<int3>());    break;
					case EFieldType::Int4:    ImGuiUtils::DrawInt4  (field->GetName(), field->GetValue<int4>(),    field->GetDefaultValue<int4>());    break;
					case EFieldType::UInt32:  ImGuiUtils::DrawUInt  (field->GetName(), field->GetValue<uint32>(),  field->GetDefaultValue<uint32>());  break;
					case EFieldType::UInt2:   ImGuiUtils::DrawUInt2 (field->GetName(), field->GetValue<uint2>(),   field->GetDefaultValue<uint2>());   break;
					case EFieldType::UInt3:   ImGuiUtils::DrawUInt3 (field->GetName(), field->GetValue<uint3>(),   field->GetDefaultValue<uint3>());   break;
					case EFieldType::UInt4:   ImGuiUtils::DrawUInt4 (field->GetName(), field->GetValue<uint4>(),   field->GetDefaultValue<uint4>());   break;
					case EFieldType::String:
						break;
                }
            }
        });
    }

}
