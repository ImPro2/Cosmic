#include "Base/Macros.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include "EntryPoint/StartupArgumentParser.hpp"
#include "App/File.hpp"
#include "cspch.hpp"
#include "EditorApp.hpp"
#include <entt/entt.hpp>

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

        Ref<EditorModule> editorModule = ModuleSystem::AddFront<EditorModule>();

        if (mProjectToOpen.GetString() != "")
            editorModule->OpenProject(mProjectToOpen);

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
                    "", { },
                    [this](const String&)
                    {
                        mStartupArgumentParser.PrintHelp();
                        Application::Get()->Close();
					}
                ),
                StartupArgument(
                    EStartupArgumentType::Option,
                    "Project File", "-p", "--project", "General",
                    "Open a Cosmic Project file (*.cosmic)",
                    "", { },
                    [this](const String& path)
                    {
                        mProjectToOpen = path;

                        if (!FileSystem::Exists(mProjectToOpen))
                        {
                            OS::Print(std::format("Invalid project path `{}`\n", mProjectToOpen.GetString().c_str()).c_str());
                            Application::Get()->Close();
                        }
                    }
                )
            },
            []()
            {
                Application::Get()->Close();
            }
        );

        mStartupArgumentParser.Parse(spec);
        mStartupArgumentParser.CallRegisteredStartupArguments();
    }

    Application* CreateApplication(StartupArgumentList&& args)
    {
        return new EditorApp(std::move(args));
    }

}
