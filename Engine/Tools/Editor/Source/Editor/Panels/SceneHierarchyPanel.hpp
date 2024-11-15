#pragma once
#include <imgui.h>
#include <entt/entt.hpp>

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
#include "App/Event/EditorEvents.hpp"

namespace Cosmic
{

    class SceneHierarchyPanel : public Panel
    {
    public:
        SceneHierarchyPanel(const Ref<Scene>& scene);

    public:
        void OnEvent(const Event& e) override;
        bool OnMouseButtonClick(const MouseButtonClickEvent& e);
        void OnImGuiRender() override;

    public:
        const Vector<Entity>& GetSelectedEntities() const { return mSelectedEntities; }
        Vector<Entity>        GetSelectedEntities()       { return mSelectedEntities; }

    private:
        bool OnEditorSceneOpened(const EditorSceneOpenedEvent& e);

    private:
        void RenderEntities();
        void RenderEntity(Entity entity, int32 index);

    private:
        Ref<Scene> mScene;

        Entity mLastSelectedEntity;
        int32 mLastSelectedEntityIndex;
        Vector<Entity> mSelectedEntities;

        bool mClicked = false;
    };

}
