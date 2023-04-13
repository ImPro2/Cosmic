module;
#include "cspch.hpp"
#include <glm/glm.hpp>
#include <entt/entt.hpp>
export module Editor.EditorModule;

import Editor.Panels;
import Editor.Panels.ViewportPanel;

import Cosmic.App;
import Cosmic.Base;
import Cosmic.Time.DeltaTime;
import Cosmic.Time;
import Cosmic.Renderer.RenderCommand;
import Cosmic.Renderer.Texture;
import Cosmic.Renderer.Framebuffer;
import Cosmic.Renderer.Renderer2D;
import Cosmic.Renderer.OrthographicCamera;
import Cosmic.Renderer.Camera;

import Cosmic.ECS.Entity;
import Cosmic.ECS.Scene;
import Cosmic.ECS.Components;
import Cosmic.Script.NativeScript;

import Cosmic.App.FileSystemEvents;
import Cosmic.App.WindowEvents;
import Cosmic.App.EditorEvents;

import Editor.TabBars;

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
        void SetupMenuBar();

    private:
        bool OnKeyPressed(const KeyPressEvent& e);
        bool OnFileModified(const FileModifiedEvent& e);

    private:
        void SaveScene();
        void SaveSceneAs();
        void OpenScene();
        void NewScene();

    private:
        Ref<Scene> mActiveScene;
        String     mActiveScenePath;

        Ref<Texture2D> mCosmicLogoTexture;
        Ref<Framebuffer> mFramebuffer;

        Panels mPanels;

        Vector<ITabBar> mTabBars;
    };

}