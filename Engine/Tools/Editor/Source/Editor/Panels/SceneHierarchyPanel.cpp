#include "App/KeyAndMouseCodes.hpp"
#include "cspch.hpp"
#include "SceneHierarchyPanel.hpp"
#include <imgui.h>
#include <entt/entt.hpp>
#include <IconsFontAwesome6.h>
#include "Editor/Event/EditorSceneEvents.hpp"
#include "Editor/EditorModule.hpp"

#include "ECS/Components.hpp"
#include "imgui_internal.h"

CS_MODULE_LOG_INFO(Editor, Panels.SceneHierarchyPanel);

namespace Cosmic
{

    SceneHierarchyPanel::SceneHierarchyPanel()
        : Panel("Scene Hierarchy")
    {
    }

    void SceneHierarchyPanel::OnInit()
    {
        EditorModule* editorModule = ModuleSystem::Get<EditorModule>();
        mScene = editorModule->GetActiveScene();
    }

    void SceneHierarchyPanel::OnEvent(const Event& e)
    {
        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(KeyPressEvent, OnKeyPressed);
        CS_DISPATCH_EVENT(EditorSceneOpenedEvent, OnEditorSceneOpened);
    }

    bool SceneHierarchyPanel::OnKeyPressed(const KeyPressEvent& e)
    {
        bool control = Input::IsKeyPressed(EKeyCode::LeftControl) || Input::IsKeyPressed(EKeyCode::RightControl);

        switch (e.GetKeyCode())
        {
            case EKeyCode::A:
            {
                if (control)
                    SelectAllEntities();
                break;
            }
            case EKeyCode::N:
            {
                if (control)
                    AddNewEntity();
                break;
            }
            case EKeyCode::X:
            {
                if (control)
                    DeleteSelectedEntities();
                break;
            }
            case EKeyCode::D:
            {
                if (control)
                    DuplicateSelectedEntities();
                break;
            }
        }
        
        return false;
    }

    bool SceneHierarchyPanel::OnEditorSceneOpened(const EditorSceneOpenedEvent& e)
    {
        mScene = e.GetScene();
        mSelectedEntities.clear();
        mLastSelectedEntity = {};
        mLastSelectedEntityIndex = -1;

        return true;
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

            RenderEntities();
            RenderRightClickMenu();

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

            if ((ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseDown(ImGuiMouseButton_Right)) && ImGui::IsItemHovered())
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

    void SceneHierarchyPanel::RenderRightClickMenu()
    {
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByPopup))
            ImGui::OpenPopup(sPopupID);

        if (ImGui::BeginPopup(sPopupID))
        {
            bool entitiesSelected = !mSelectedEntities.empty();

            if (ImGui::MenuItem("Select All", "CTRL+A"))
            {
                SelectAllEntities();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Add Entity", "CTRL+N"))
            {
                AddNewEntity();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Duplicate", "CTRL+D", false, entitiesSelected))
            {
                DuplicateSelectedEntities();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Delete", "CTRL+X", false, entitiesSelected))
            {
                DeleteSelectedEntities();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void SceneHierarchyPanel::SelectAllEntities()
    {
        mSelectedEntities.clear();

        mScene->ForEachEntity([this](Entity entity)
        {
            mSelectedEntities.push_back(entity);
        });

        if (!mSelectedEntities.empty())
        {
            mLastSelectedEntityIndex = mSelectedEntities.size() - 1;
            mLastSelectedEntity = mSelectedEntities[mLastSelectedEntityIndex];
        }
    }

    void SceneHierarchyPanel::AddNewEntity()
    {
        mLastSelectedEntity = mScene->CreateEntity("New Entity");

        mSelectedEntities = { mLastSelectedEntity };
        mLastSelectedEntityIndex = 0;
    }

    void SceneHierarchyPanel::DeleteSelectedEntities()
    {
        for (Entity entity : mSelectedEntities)
        {
            mScene->RemoveEntity(entity);
            EventSystem::AddEvent(new EditorEntityRemovedEvent(entity, mScene));
        }

        mSelectedEntities.clear();
        mLastSelectedEntity = {};
        mLastSelectedEntityIndex = -1;
    }

    void SceneHierarchyPanel::DuplicateSelectedEntities()
    {
        int32 count = mSelectedEntities.size();
        for (int32 i = 0; i < count; i++)
        {
            mSelectedEntities.emplace_back(mScene->CreateEntity(mSelectedEntities[i].GetComponent<TagComponent>().Tag));
            EventSystem::AddEvent(new EditorEntityAddedEvent(mSelectedEntities[count + i], mScene));
        }
    }

}
