#pragma once
#include <entt/entt.hpp>

#include "Panels.hpp"

#include "App/Module.hpp"
#include "Base/Base.hpp"
#include "Renderer/Framebuffer.hpp"
#include "Renderer/OrthographicCameraController.hpp"
#include "Time/DeltaTime.hpp"
#include "Gui/Gui.hpp"
#include "App/Event/Events.hpp"
#include "ECS/Scene.hpp"

#include "Editor/Event/EditorSceneEvents.hpp"
#include "Editor/EditorCamera.hpp"

namespace Cosmic
{

    class ViewportPanel : public Panel
    {
    public:
        ViewportPanel();

    public:
        void OnInit() override;
        void OnUpdate(Dt dt) override;
        void OnEvent(const Event& e) override;
        void OnImGuiRender() override;

    private:
        bool OnEditorSceneOpened(const EditorSceneOpenedEvent& e);

    private:
        Ref<Framebuffer> mFramebuffer;
        Ref<Scene>       mScene;
        bool mSceneChanged = false;
        bool mWindowHovered = false;

        EditorCamera mCamera;
    };

}
