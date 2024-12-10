#pragma once
#include "Editor/EditorModule.hpp"

#include "App/App.hpp"
#include "EntryPoint/EntryPoint.hpp"

#include "Event/Events.hpp"
#include "Event/Type/AppEvents.hpp"

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
