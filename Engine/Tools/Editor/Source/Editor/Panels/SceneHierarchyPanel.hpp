#pragma once
#include "Panels.hpp"

#include "Event/Type/WindowEvents.hpp"
#include "Editor/Event/EditorEvents.hpp"
#include "Editor/Event/SceneEvents.hpp"
#include "Editor/Event/EntityEvents.hpp"

#include "Base/Base.hpp"
#include "App/Module.hpp"
#include "App/Log/Log.hpp"
#include "App/Input.hpp"
#include "App/KeyAndMouseCodes.hpp"
#include "Time/DeltaTime.hpp"
#include "ECS/Scene.hpp"
#include "ECS/Entity.hpp"

#include <imgui.h>
#include <entt/entt.hpp>

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
        bool OnEditorSceneOpened(const SceneOpenedEvent& e);
        bool OnEntityAdded(const EntityAddedEvent& e);
        bool OnEntityRemoved(const EntityRemovedEvent& e);

    private:
        void RenderEntities();
        void RenderEntity(Entity entity, int32& index);
        void RenderRightClickMenu();

    private:
        void SelectAllEntities();
        void AddNewEntity();
        void DeleteSelectedEntities();
        void DuplicateSelectedEntities();

    private:
        Ref<Scene> mScene;

        Vector<Entity> mSelectedEntities;

        bool mWindowHovered = false;

        ImVec2 mMouseSelectionStart, mMouseSelectionEnd;
        bool mMouseSelectionStarted;

        static constexpr const char* sPopupID = "Scene Hierarchy Panel Right Click";
    };

}
