#include "App/KeyAndMouseCodes.hpp"
#include "cspch.hpp"
#include "SceneHierarchyPanel.hpp"
#include <imgui.h>
#include <entt/entt.hpp>
#include <IconsFontAwesome6.h>

#include "ECS/Components.hpp"
#include "imgui_internal.h"

CS_MODULE_LOG_INFO(Editor, Panels.SceneHierarchyPanel);

namespace Cosmic
{

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
        : Panel("Scene Hierarchy")
    {
        mScene = scene;
    }

    void SceneHierarchyPanel::OnEvent(const Event& e)
    {
        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(MouseButtonClickEvent, OnMouseButtonClick);
        CS_DISPATCH_EVENT(EditorSceneOpenedEvent, OnEditorSceneOpened);
    }

    bool SceneHierarchyPanel::OnMouseButtonClick(const MouseButtonClickEvent& e)
    {
        mClicked = true;
        return false;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        if (!mOpen)
            return;

        if (ImGui::Begin(mPanelName.c_str(), &mOpen))
        {
            char tag[128] = "";

            if (ImGui::InputTextWithHint("Add Entity", "Enter Tag Here", tag, 128, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                mLastSelectedEntity = mScene->CreateEntity(tag);
                mSelectedEntities.clear();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
            ImGui::BeginChild("Entities", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);

#if 0
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
                    flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
                else
                    flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
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
                    mSelectedEntity = Entity();
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

#endif

            RenderEntities();

            ImGui::PopStyleVar(2);
            ImGui::EndChild();
        }
        ImGui::End();

        mClicked = false;
    }

    void SceneHierarchyPanel::RenderEntities()
    {
        ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        if (ImGui::BeginTable("Entities", 2, tableFlags))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, ImGui::GetContentRegionAvailWidth() - 20.0f);
            ImGui::TableSetupColumn(ICON_FA_EYE, ImGuiTableColumnFlags_WidthFixed, 20.0f);
            ImGui::TableHeadersRow();

            mScene->ForEachEntityIndexed([this](Entity e, int32 i) { this->RenderEntity(e, i); });

            ImGui::EndTable();
        }
    }

    void SceneHierarchyPanel::RenderEntity(Entity entity, int32 index)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        auto findEntity = [](Vector<Entity>& entities, Entity entity) -> Vector<Entity>::iterator
        {
            for (Vector<Entity>::iterator it = entities.begin(); it != entities.end();)
            {
                if (entity == *it)
                    return it;

                ++it;
            }

            return entities.end();
        };
        
        bool hasChildren = false;

        const String& tag       = entity.GetComponent<TagComponent>().Tag;
        bool&         isVisible = entity.GetComponent<EntityMetadataComponent>().IsVisible;

        if (!hasChildren)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

            // Set the entity selected if it is selected

            if (findEntity(mSelectedEntities, entity) != mSelectedEntities.end())
                flags |= ImGuiTreeNodeFlags_Selected;

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));

            bool open = ImGui::TreeNodeEx(tag.c_str(), flags);

            ImGui::PopStyleColor();
            ImGui::PopStyleVar();

            if (open)
            {
                ImGui::TreePop();
            }

            // Entity Selection

            if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
            {
                mLastSelectedEntity = entity;

                //if (ImGui::GetIO().KeyCtrl)
                if (Input::IsKeyPressed(EKeyCode::LeftControl) || Input::IsKeyPressed(EKeyCode::RightControl))
                {
                    // Adds entities to the selection one by one, or deselects them if they're already selected.

                    auto entityIter = findEntity(mSelectedEntities, entity);

                    if (entityIter != mSelectedEntities.end())
                        mSelectedEntities.erase(entityIter);
                    else
                        mSelectedEntities.push_back(entity);
                }
                else if (Input::IsKeyPressed(EKeyCode::LeftShift) || Input::IsKeyPressed(EKeyCode::RightShift))
                {
                    // Adds more than one entities to the selection.

                    mScene->ForEachEntityIndexed([&](Entity other, int32 i)
                    {
                        if (index > mLastSelectedEntityIndex && (i > mLastSelectedEntityIndex && i <= index))
                        {
                            mSelectedEntities.push_back(other);
                        }
                        else if (index < mLastSelectedEntityIndex && (i >= index && i < mLastSelectedEntityIndex))
                        {
                            mSelectedEntities.push_back(other);
                        }
                    });
                }
                else
                {
                    // A single left click clears the selection and only selects one entity.

                    mSelectedEntities.clear();
                    mSelectedEntities.push_back(entity);
                }

                mLastSelectedEntityIndex = index;
            }

            ImGui::TableNextColumn();
            ImGui::PushID(index);
            
            // Hack to make the radio button smaller

            ImFont* currFont = ImGui::GetFont(); 
            currFont->Scale = 0.75f;
            ImGui::PushFont(currFont);

            if (ImGui::RadioButton("", isVisible))
                isVisible = !isVisible;
            
            currFont->Scale = 1.0f;
            ImGui::PopFont();

            ImGui::PopID();
        }
    }

    bool SceneHierarchyPanel::OnEditorSceneOpened(const EditorSceneOpenedEvent& e)
    {
        mScene = e.GetScene();
        return true;
    }

}
