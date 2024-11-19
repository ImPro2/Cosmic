#pragma once
#include "App/App.hpp"
#include "App/Event/FileSystemEvents.hpp"
#include "App/File.hpp"
#include "App/Path.hpp"
#include "Base/Base.hpp"
#include "Time/Time.hpp"
#include "Time/DeltaTime.hpp"
#include "Renderer/Renderer.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Scene.hpp"
#include "ECS/Components.hpp"
#include "Script/NativeScript.hpp"

#include "Panels/Panels.hpp"
#include "Panels/ViewportPanel.hpp"
#include "Event/EditorEvents.hpp"
#include "Event/EditorSceneEvents.hpp"

#include <glm/glm.hpp>
#include <entt/entt.hpp>

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

    public:
        void SaveScene();
        void SaveSceneAs(File file);
        void SaveSceneAs();
        void OpenScene(File file);
        void OpenScene();
        void NewScene();

    private:
        void SetupMenuBar();
        void SetupDockSpace();
        void SetupDefaultDockLayout();

    private:
        bool OnKeyPressed(const KeyPressEvent& e);
        bool OnFileModified(const FileModifiedEvent& e);

    private:
        Ref<Scene> mActiveScene;
        String     mActiveScenePath;

        Ref<Texture2D> mCosmicLogoTexture;
        Ref<Framebuffer> mFramebuffer;

        Panels mPanels;

        bool mSetupDefaultLayout = true;
        
        bool mShowDemoWindow = false;
    };

}
