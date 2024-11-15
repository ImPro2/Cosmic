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
#include "App/Event/EditorEvents.hpp"

namespace Cosmic
{

    class ViewportPanel : public Panel
    {
    public:
        ViewportPanel(const Ref<Framebuffer>& framebuffer, const Ref<Scene>& scene);

    public:
        void OnUpdate(Dt dt) override;
        void OnEvent(const Event& e) override;
        void OnImGuiRender() override;

    public:
        const OrthographicCameraController& GetCameraController() const { return mCameraController; }

    private:
        bool OnEditorSceneOpened(const EditorSceneOpenedEvent& e);

    private:
        Ref<Framebuffer> mFramebuffer;
        Ref<Scene>       mScene;
        OrthographicCameraController mCameraController;
        bool mSceneChanged = false;
    };

}