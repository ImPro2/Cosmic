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

namespace Cosmic
{

    class SceneHierarchyPanel : public Panel
    {
    public:
        SceneHierarchyPanel();

    public:
        void OnInit() override;
        void OnEvent(const Event& e) override;
        void OnImGuiRender() override;

    public:
        const Vector<Entity>& GetSelectedEntities() const { return mSelectedEntities; }
        Vector<Entity>        GetSelectedEntities()       { return mSelectedEntities; }

    private:
        bool OnKeyPressed(const KeyPressEvent& e);
        bool OnEditorSceneOpened(const EditorSceneOpenedEvent& e);

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

        bool mClicked = false;

        static constexpr const char* sPopupID = "Scene Hierarchy Panel Right Click";
    };

}
