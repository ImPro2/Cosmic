#include "Base/Macros.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include "EntryPoint/StartupArgumentParser.hpp"
#include "App/File.hpp"
#include "cspch.hpp"
#include "EditorApp.hpp"
#include <entt/entt.hpp>

//#include "Project/ProjectModule.hpp"

CS_MODULE_LOG_INFO(Editor, EditorApp)

namespace Cosmic
{

    EditorApp::EditorApp(const StartupArgumentList& args)
        : mStartupArgumentParser(args)
    {
        ApplicationInfo info = {};
        info.StartupArgs     = args;

        SetupStartupArguments();
        Init(info);
    }

    bool EditorApp::OnInit(const ApplicationInitEvent& e)
    {
        CS_PROFILE_FN();

        ModuleSystem::AddFront<EditorModule>();
        return false;
    }

    void EditorApp::OnEvent(const IEvent& e)
    {
        CS_PROFILE_FN();

        Application::OnEvent(e);

        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(ApplicationInitEvent, OnInit);
    }

    void EditorApp::SetupStartupArguments()
    {
        StartupArgumentSpecification spec = StartupArgumentSpecification(
            "Cosmic",
            "A game engine",
            "Program Brief",
            { "General"},
            {
                StartupArgument(
                    EStartupArgumentType::Flag,
                    "Help", "-h", "--help", "General",
                    "Print this help message",
                    { },
                    [this](const String&)
                    {
                        mStartupArgumentParser.PrintHelp();
					}
                ),
                StartupArgument(
                    EStartupArgumentType::Option,
                    "Project File", "-p", "--project", "General",
                    "Open a Cosmic Project file (*.cosmic)",
                    { },
                    [](const String& path)
                    {
                        ModuleSystem::Get<EditorModule>()->OpenProject(Path(path));
                    }
                )
            }
        );

        mStartupArgumentParser.Parse(spec);
        mStartupArgumentParser.PrintHelp();
    }

    Application* CreateApplication(StartupArgumentList&& args)
    {
        return new EditorApp(std::move(args));
    }

}
