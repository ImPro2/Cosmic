#include "Base/Macros.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include "cspch.hpp"
#include "EditorApp.hpp"
#include <entt/entt.hpp>

//#include "Project/ProjectModule.hpp"

CS_MODULE_LOG_INFO(Editor, EditorApp)

namespace Cosmic
{

    EditorApp::EditorApp(const StartupArguments& args)
    {
        ApplicationInfo info = {};
        info.StartupArgs = args;

        Init(info);
    }

    bool EditorApp::OnInit(const ApplicationInitEvent& e)
    {
        CS_PROFILE_FN();

        ModuleSystem::AddFront<EditorModule>();

        return false;
    }

    void EditorApp::OnEvent(const Event& e)
    {
        CS_PROFILE_FN();

        Application::OnEvent(e);

        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(ApplicationInitEvent, OnInit);
    }

    Application* CreateApplication(StartupArguments&& args)
    {
        return new EditorApp(args);
    }

}
