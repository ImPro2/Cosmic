module;
#include "cspch.hpp"
#include <glm/glm.hpp>
#include <entt/entt.hpp>
export module Editor.EditorModule;

import Editor.Panels;
import Editor.Panels.ViewportPanel;

import Cosmic.App;
import Cosmic.Time.DeltaTime;
import Cosmic.Time;
import Cosmic.Renderer.RenderCommand;
import Cosmic.Renderer.Texture;
import Cosmic.Renderer.Framebuffer;
import Cosmic.Renderer.Renderer2D;
import Cosmic.Renderer.OrthographicCamera;

import Cosmic.ECS.Scene;
import Cosmic.ECS.Components;

namespace Cosmic
{

    export class EditorModule : public Module
    {
    public:
        void OnInit() override;
        void OnShutdown() override;
        void OnUpdate(Dt dt) override;
        void OnEvent(const Event& e) override;
        void OnImGuiRender() override;

    private:
        void SetupDockSpace();

    private:
        Ref<Scene> mActiveScene;

        Ref<Texture2D> mCosmicLogoTexture;
        Ref<Framebuffer> mFramebuffer;

        Panels mPanels;
    };

}