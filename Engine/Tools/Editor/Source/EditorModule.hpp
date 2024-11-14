#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "Panels/Panels.hpp"
#include "Panels/ViewportPanel.hpp"
#include "TabBars/TabBars.hpp"
#include "Gui/FileDialog.hpp"

#include "App/App.hpp"
#include "App/Event/FileSystemEvents.hpp"
#include "App/Event/EditorEvents.hpp"
#include "Base/Base.hpp"
#include "Time/Time.hpp"
#include "Time/DeltaTime.hpp"
#include "Renderer/Renderer.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Scene.hpp"
#include "ECS/Components.hpp"
#include "Script/NativeScript.hpp"

namespace Cosmic
{

    class EditorModule : public Module
    {
    public:
        void OnInit()                override;
        void OnShutdown()            override;
        void OnUpdate(Dt dt)         override;
        void OnEvent(const Event& e) override;
        void OnImGuiRender()         override;

    private:
        void SetupDockSpace();
        void SetupDefaultDockLayout();
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

        bool mSetupDefaultLayout = true;
    };

}
