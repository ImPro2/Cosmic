module;
#include "cspch.hpp"
export module Editor.EditorApp;

import Editor.EditorModule;
import Cosmic.App;

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