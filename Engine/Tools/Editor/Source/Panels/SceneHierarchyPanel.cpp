module;
#include "cspch.hpp"
#include <imgui.h>
#include <entt/entt.hpp>
#include <IconsFontAwesome6.h>
module Editor.Panels.SceneHierarchyPanel;

CS_MODULE_LOG_INFO(Editor, Panels.SceneHierarchyPanel);

namespace Cosmic
{

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
    {
        mScene = scene;
    }

    void SceneHierarchyPanel::OnEvent(const Event& e)
    {
        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(MouseButtonClickEvent, OnMouseButtonClick);
    }

    bool SceneHierarchyPanel::OnMouseButtonClick(const MouseButtonClickEvent& e)
    {
        mClicked = true;
        return false;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        if (ImGui::Begin("Scene Hierarchy"), &mOpen)
        {
            char tag[128] = "";

            if (ImGui::InputTextWithHint("Add Entity", "Enter Tag Here", tag, 128, ImGuiInputTextFlags_EnterReturnsTrue))
                mSelectedEntity = mScene->CreateEntity(tag);

            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
            ImGui::BeginChild("Entities", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);

            ImGuiTreeNodeFlags treeNodeFlags =
                ImGuiTreeNodeFlags_OpenOnArrow |
                ImGuiTreeNodeFlags_OpenOnDoubleClick |
                ImGuiTreeNodeFlags_SpanAvailWidth |
                ImGuiTreeNodeFlags_SpanFullWidth;

            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

            int index                  = 0;
            int selectedEntityIndex    = -1;
            static int32 selectionMask = (1 << 2);
            static bool b = false;
            static bool openContextMenu = false;

            mScene->ForEachEntity([&](Entity entity)
            {
                bool        hasChildren = false;
                bool        isSelected  = (selectionMask & (1 << index)) != 0;
                auto        flags       = treeNodeFlags;
                const char* text        = entity.GetComponent<TagComponent>().Tag.c_str();

                if (!hasChildren)
                    flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                if (isSelected)
                    flags |= ImGuiTreeNodeFlags_Selected;

                bool duplicateEntity = Input::IsKeyPressed(EKeyCode::LeftControl) && Input::IsKeyPressed(EKeyCode::D) && isSelected;
                bool deleteEntity    = Input::IsKeyPressed(EKeyCode::LeftControl) && Input::IsKeyPressed(EKeyCode::X) && isSelected;

                ImGui::TreeNodeEx((void*)(intptr_t)index, flags, text, index);

                if (openContextMenu && ImGui::BeginPopupContextItem())
                {
                    if (ImGui::Selectable("Duplicate          ", false))
                    {
                        duplicateEntity |= true;
                        ImGui::CloseCurrentPopup();
                        openContextMenu = false;
                    }
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "CTRL+D");
                    if (ImGui::Selectable("Delete          ", false))
                    {
                        deleteEntity |= true;
                        CS_LOG_INFO("Delete");
                        ImGui::CloseCurrentPopup();
                        openContextMenu = false;
                    }
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "CTRL+X");

                    ImGui::EndPopup();
                }

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_None) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    CS_LOG_INFO("Entity {} is selected.", text);
                    mSelectedEntity     = entity;
                    selectedEntityIndex = index;
                    b |= true;
                }

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_None) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                    openContextMenu = true;

                if (duplicateEntity)
                {
                    // TODO
                    CS_LOG_INFO("Duplicated Entity {}", text);
                }

                if (deleteEntity)
                {
                    mScene->RemoveEntity(entity);
                    CS_LOG_INFO("Deleted Entity {}", text);
                }

                index++;
            });

            if (selectedEntityIndex != -1)
            {
                if (ImGui::GetIO().KeyCtrl)
                    selectionMask ^= (1 << selectedEntityIndex);
                else
                    selectionMask = (1 << selectedEntityIndex);
            }
            if (!b && mClicked)
            {
                selectionMask = (1 << 2);
            }
            else
                b = false;

            ImGui::PopStyleVar(2);
            ImGui::EndChild();
        }
        ImGui::End();

        mClicked = false;
    }

}