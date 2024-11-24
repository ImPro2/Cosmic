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

#include "Editor/Action/ActionManager.hpp"

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace Cosmic
{

    class EditorModule : public IModule
    {
    public:
        void OnInit()                 override;
        void OnShutdown()             override;
        void OnUpdate(Dt dt)          override;
        void OnEvent(const IEvent& e) override;
        void OnImGuiRender()          override;

    public:
        void SaveScene();
        void SaveSceneAs(File file);
        void SaveSceneAs();
        void OpenScene(File file);
        void OpenScene();
        void NewScene();

    public:
        const Ref<Scene>& GetActiveScene() const { return mActiveScene; }
        Ref<Scene>        GetActiveScene()       { return mActiveScene; }

        const String& GetActiveScenePath() const { return mActiveScenePath; }

        const Panels& GetPanels() const { return mPanels; }
        Panels&       GetPanels()       { return mPanels; }

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

        Panels        mPanels;
        ActionManager mActionManager;

        bool mSetupDefaultLayout = true;
        bool mShowDemoWindow = false;
    };

}
