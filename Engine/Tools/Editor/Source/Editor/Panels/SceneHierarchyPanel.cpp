#include "App/KeyAndMouseCodes.hpp"
#include "cspch.hpp"
#include "SceneHierarchyPanel.hpp"
#include <imgui.h>
#include <entt/entt.hpp>
#include <IconsFontAwesome6.h>
#include "Editor/Event/SceneEvents.hpp"
#include "Editor/EditorModule.hpp"
#include "UI/ImGuiUtils/ImGuiUtils.hpp"

#include "ECS/Components.hpp"
#include "imgui_internal.h"

CS_MODULE_LOG_INFO(Editor, Panels.SceneHierarchyPanel);

namespace Cosmic
{

    SceneHierarchyPanel::SceneHierarchyPanel()
        : IPanel("Scene Hierarchy")
    {
        mSelectedEntities.clear();
    }

    void SceneHierarchyPanel::OnInit()
    {
        Ref<EditorModule> editorModule = ModuleSystem::Get<EditorModule>();
        mScene = editorModule->GetActiveScene();
    }

    void SceneHierarchyPanel::OnEvent(const IEvent& e)
    {
        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(KeyPressEvent, OnKeyPressed);
        CS_DISPATCH_EVENT(SceneOpenedEvent, OnEditorSceneOpened);
        CS_DISPATCH_EVENT(EntityAddedEvent, OnEntityAdded);
        CS_DISPATCH_EVENT(EntityRemovedEvent, OnEntityRemoved);
    }

    bool SceneHierarchyPanel::OnKeyPressed(const KeyPressEvent& e)
    {
        if (!mWindowHovered)
            return false;

        bool control = Input::IsKeyPressed(EKeyCode::LeftControl) || Input::IsKeyPressed(EKeyCode::RightControl);

        switch (e.GetKeyCode())
        {
			case EKeyCode::A: if (control) SelectAllEntities();         break;
			case EKeyCode::N: if (control) AddNewEntity();              break;
			case EKeyCode::X: if (control) DeleteSelectedEntities();    break;
			case EKeyCode::D: if (control) DuplicateSelectedEntities(); break;
        }

        return false;
    }

    bool SceneHierarchyPanel::OnEditorSceneOpened(const SceneOpenedEvent& e)
    {
        mScene = e.GetScene();
        mSelectedEntities.clear();

        return true;
    }

    bool SceneHierarchyPanel::OnEntityAdded(const EntityAddedEvent& e)
    {
        return false;
    }

    bool SceneHierarchyPanel::OnEntityRemoved(const EntityRemovedEvent& e)
    {
        for (Entity entity : e.GetEntities())
            e.GetScene()->RemoveEntity(entity);

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
                mSelectedEntities = { mScene->CreateEntity(tag) };

            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
            ImGui::BeginChild("Entities", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);

            RenderEntities();
            RenderRightClickMenu();

            if (ImGui::IsWindowHovered() || mMouseSelectionStarted)
            {
                bool first = !mMouseSelectionStarted;
                mMouseSelectionStarted = !ImGuiUtils::SelectionRect(&mMouseSelectionStart, &mMouseSelectionEnd, ImGuiMouseButton_Left);

                if (first && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    mSelectedEntities.clear();
            }

            ImGui::PopStyleVar(2);
            ImGui::EndChild();
        }

        mWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

        ImGui::End();
    }

    void SceneHierarchyPanel::RenderEntities()
    {
        ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        if (ImGui::BeginTable("Entities", 2, tableFlags))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, ImGui::GetContentRegionAvailWidth() - 20.0f);
            ImGui::TableSetupColumn(ICON_FA_EYE, ImGuiTableColumnFlags_WidthFixed, 20.0f);
            ImGui::TableHeadersRow();

            //mScene->ForEachEntityIndexed([this](Entity e, int32 i) { RenderEntity(e, i); });

            int32 i = 0;
            mScene->ForEachRootEntity([this, &i](Entity entity) { RenderEntity(entity, i); i++; });

            ImGui::EndTable();
        }
    }

    void SceneHierarchyPanel::RenderEntity(Entity entity, int32& index)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        auto findEntity = [this](Entity entity) -> Vector<Entity>::iterator
        {
            for (Vector<Entity>::iterator it = mSelectedEntities.begin(); it != mSelectedEntities.end(); it++)
            {
                if (entity == *it)
                    return it;
            }

            return mSelectedEntities.end();
        };

        auto& metadata = entity.GetComponent<EntityMetadataComponent>();

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

        // Set the entity selected if it is selected

        if (findEntity(entity) != mSelectedEntities.end())
            flags |= ImGuiTreeNodeFlags_Selected;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));

        bool open    = ImGui::TreeNodeEx(metadata.Tag.c_str(), flags);
        bool clicked = ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right);

        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        // Entity Selection

        if (mMouseSelectionStarted && findEntity(entity) == mSelectedEntities.end())
        {
            ImVec2 topLeft  = ImGui::GetItemRectMin();
            ImVec2 btmRight = ImGui::GetItemRectMax();

            ImVec2 mouseTopLeft  = ImVec2(std::min(mMouseSelectionStart.x, mMouseSelectionEnd.x), std::min(mMouseSelectionStart.y, mMouseSelectionEnd.y));
            ImVec2 mouseBtmRight = ImVec2(std::max(mMouseSelectionStart.x, mMouseSelectionEnd.x), std::max(mMouseSelectionStart.y, mMouseSelectionEnd.y));

            if ((topLeft.y > mouseTopLeft.y && topLeft.y < mouseBtmRight.y) ||
                (btmRight.y > mouseTopLeft.y && btmRight.y < mouseBtmRight.y))
            {
                mSelectedEntities.push_back(entity);
            }
        }

        if (clicked)
        {
            if (Input::IsKeyPressed(EKeyCode::LeftControl) || Input::IsKeyPressed(EKeyCode::RightControl))
            {
                // Adds entities to the selection one by one, or deselects them if they're already selected.

                auto entityIter = findEntity(entity);

                if (entityIter != mSelectedEntities.end())
                    mSelectedEntities.erase(entityIter);
                else
                    mSelectedEntities.push_back(entity);
            }
            else if (Input::IsKeyPressed(EKeyCode::LeftShift) || Input::IsKeyPressed(EKeyCode::RightShift))
            {
                // Adds more than one entities to the selection.

                int32 lastSelectedEntityIndex = mSelectedEntities.size() - 1;

                mScene->ForEachEntityIndexed([&](Entity other, int32 i)
				{
					if (index > lastSelectedEntityIndex && (i > lastSelectedEntityIndex && i <= index))
					{
						mSelectedEntities.push_back(other);
					}
					else if (index < lastSelectedEntityIndex && (i >= index && i < lastSelectedEntityIndex))
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
        }

        // Render IsVisible radio button

        ImGui::TableNextColumn();
        ImGui::PushID(index);

        // Hack to make the radio button smaller

        ImFont* currFont = ImGui::GetFont();
        currFont->Scale = 0.75f;
        ImGui::PushFont(currFont);

        if (ImGui::RadioButton("", metadata.IsVisible))
            metadata.IsVisible = !metadata.IsVisible;

        currFont->Scale = 1.0f;
        ImGui::PopFont();
        ImGui::PopID();

        // Render children

        if (open)
        {
            Entity child = metadata.FirstChild;

            while (child)
            {
                index++;
                RenderEntity(child, index);

                child = child.GetComponent<EntityMetadataComponent>().Next;
            }

            ImGui::TreePop();
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

    void SceneHierarchyPanel::ClearSelectedEntities()
    {
        mSelectedEntities.clear();
    }

    void SceneHierarchyPanel::SetSelectedEntities(const Vector<Entity>& entities)
    {
        ClearSelectedEntities();
        mSelectedEntities = entities;
    }

    void SceneHierarchyPanel::SelectAllEntities()
    {
        mSelectedEntities.clear();

        mScene->ForEachEntity([this](Entity entity)
        {
            mSelectedEntities.push_back(entity);
        });
    }

    void SceneHierarchyPanel::AddNewEntity()
    {
        mSelectedEntities = { mScene->CreateEntity() };

        EventSystem::DeferEvent<EntityAddedEvent>(mSelectedEntities, mScene);
    }

    void SceneHierarchyPanel::DeleteSelectedEntities()
    {
        EventSystem::DeferEvent<EntityRemovedEvent>(mSelectedEntities, mScene);

        mSelectedEntities.clear();
    }

    void SceneHierarchyPanel::DuplicateSelectedEntities()
    {
        Vector<Entity> duplicatedEntities(mSelectedEntities.size());

        for (Entity entity : mSelectedEntities)
        {
            duplicatedEntities.push_back(mScene->AddEntity(entity));
        }

        mSelectedEntities.insert(mSelectedEntities.end(), duplicatedEntities.begin(), duplicatedEntities.end());

		EventSystem::DeferEvent<EntityAddedEvent>(duplicatedEntities, mScene);
    }

}
