#include "cspch.hpp"
#include "EditorApp.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include <entt/entt.hpp>

#include "ProjectModule.hpp"

namespace Cosmic
{

    EditorApp::EditorApp()
    {
        Init({});
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

    Application* CreateApplication()
    {
        return new EditorApp();
    }

}