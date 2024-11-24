#pragma once
#include "Editor/EditorModule.hpp"

#include "App/App.hpp"
#include "EntryPoint/EntryPoint.hpp"

namespace Cosmic
{

    class EditorApp : public Application
    {
    public:
        EditorApp(const StartupArguments& args);

        bool OnInit(const ApplicationInitEvent& e);
        void OnEvent(const IEvent& e) override;
    };

}
