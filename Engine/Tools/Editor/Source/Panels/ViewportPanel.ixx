module;
#include "cspch.hpp"
#include <entt/entt.hpp>
export module Editor.Panels.ViewportPanel;

import Editor.Panels;
import Cosmic.App.Module;
import Cosmic.Base;
import Cosmic.Renderer.Framebuffer;
import Cosmic.Renderer.OrthographicCameraController;
import Cosmic.Time.DeltaTime;
import Cosmic.Gui;
import Cosmic.App.Events;
import Cosmic.ECS.Scene;
import Cosmic.App.EditorEvents;

namespace Cosmic
{

    export class ViewportPanel : public Panel
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