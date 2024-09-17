#pragma once
#include "EditorModule.hpp"

#include "App/App.hpp"

namespace Cosmic
{

    class EditorApp : public Application
    {
    public:
        EditorApp();

        bool OnInit(const ApplicationInitEvent& e);
        void OnEvent(const Event& e) override;
    };

}