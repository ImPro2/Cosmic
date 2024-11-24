#pragma once
#include <imgui.h>
#include <entt/entt.hpp>

#include "Editor/Event/EditorSceneEvents.hpp"
#include "Panels.hpp"

#include "Base/Base.hpp"
#include "App/Module.hpp"
#include "App/Log/Log.hpp"
#include "App/Input.hpp"
#include "App/KeyAndMouseCodes.hpp"
#include "Time/DeltaTime.hpp"
#include "ECS/Scene.hpp"
#include "ECS/Entity.hpp"
#include "App/Event/Events.hpp"
#include "App/Event/WindowEvents.hpp"

#include "Editor/Event/EditorEvents.hpp"
#include "Editor/Event/EditorSceneEvents.hpp"

namespace Cosmic
{

    class SceneHierarchyPanel : public IPanel
    {
    public:
        SceneHierarchyPanel();

    public:
        void OnInit()                 override;
        void OnEvent(const IEvent& e) override;
        void OnImGuiRender()          override;

    public:
        void ClearSelectedEntities();
        void SetSelectedEntities(const Vector<Entity>& entities);

        const Vector<Entity>& GetSelectedEntities() const { return mSelectedEntities; }
        Vector<Entity>        GetSelectedEntities()       { return mSelectedEntities; }

    private:
        bool OnKeyPressed(const KeyPressEvent& e);
        bool OnEditorSceneOpened(const EditorSceneOpenedEvent& e);
        bool OnEntityAdded(const EditorEntityAddedEvent& e);
        bool OnEntityRemoved(const EditorEntityRemovedEvent& e);

    private:
        void RenderEntities();
        void RenderEntity(Entity entity, int32 index);
        void RenderRightClickMenu();

    private:
        void SelectAllEntities();
        void AddNewEntity();
        void DeleteSelectedEntities();
        void DuplicateSelectedEntities();

    private:
        Ref<Scene> mScene;

        Entity mLastSelectedEntity;
        int32 mLastSelectedEntityIndex;
        Vector<Entity> mSelectedEntities;

        bool mWindowHovered = false;

        ImVec2 mMouseSelectionStart, mMouseSelectionEnd;
        bool mMouseSelectionStarted;

        static constexpr const char* sPopupID = "Scene Hierarchy Panel Right Click";
    };

}
