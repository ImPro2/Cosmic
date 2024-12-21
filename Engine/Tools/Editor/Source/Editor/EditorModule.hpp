#pragma once
#include "App/App.hpp"
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

#include "Project/Project.hpp"
#include "Project/ProjectManager.hpp"

#include "Panels/Panels.hpp"
#include "Panels/ViewportPanel.hpp"
#include "Event/EditorEvents.hpp"
#include "Event/SceneEvents.hpp"
#include "Event/ProjectEvents.hpp"

#include "Editor/Action/ActionManager.hpp"

#include "Event/Events.hpp"
#include "Event/Type/FileSystemEvents.hpp"
#include "Editor/Event/SceneEvents.hpp"

#include "UI/Layout/LayoutManager.hpp"

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
        void SaveProject();
        void SaveProjectAs();
        void SaveProjectAs(File file);
        void OpenProject();
        void OpenProject(File file);
        void NewProject();
        void NewProject(File file);

        void SaveScene();
        void SaveSceneAs(File file);
        void SaveSceneAs();
        void OpenScene(File file);
        void OpenScene();
        void NewScene();

    public:
        const Ref<Scene>&    GetActiveScene()     const { return mActiveScene;     }
        Ref<Scene>           GetActiveScene()           { return mActiveScene;     }

        const File&          GetActiveSceneFile() const { return mActiveSceneFile; }

        const Panels&        GetPanels()          const { return mPanels;          }
        Panels&              GetPanels()                { return mPanels;          }

        const ActionManager& GetActionManager()   const { return mActionManager;   }
        ActionManager&       GetActionManager()         { return mActionManager;   }

        const LayoutManager& GetLayoutManager()   const { return mLayoutManager;   }
        LayoutManager&       GetLayoutManager()         { return mLayoutManager;   }

        bool* GetShowImGuiDemoWindowPtr() { return &mShowDemoWindow; }

    public:
        void ToggleShowImGuiDemoWindow() { mShowDemoWindow = !mShowDemoWindow; }

    private:
        bool OnKeyPressed(const KeyPressEvent& e);
        bool OnFileAdded(const FileAddedEvent& e);
        bool OnSceneOpened(const SceneOpenedEvent& e);
        bool OnSceneNew(const SceneNewEvent& e);

    private:
        void SetWindowTitle();

    private:
        Ref<Project> mActiveProject;

        Ref<Scene> mActiveScene;
        File       mActiveSceneFile;

        Panels        mPanels;
        ActionManager mActionManager;
        LayoutManager mLayoutManager;

        bool mShowDemoWindow = false;
    };

}
