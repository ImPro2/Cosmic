module;
#include "cspch.hpp"
#include <queue>
export module Editor.Panels.ConsolePanel;

import Editor.Panels;

import Cosmic.App.Module;
import Cosmic.Base;
import Cosmic.App.Log;
import Cosmic.App.ConsoleColor;
import Cosmic.App.LogSeverity;
import Cosmic.App.LogEvents;
import Cosmic.Base.StringUtils;

namespace Cosmic
{

    export class ConsolePanel : public Panel
    {
    public:
        ConsolePanel();

    public:
        void OnEvent(const Event& e) override;
        void OnImGuiRender() override;

    private:
        bool OnLog(const LogEvent& e);

    private:
        Vector<LogEvent> mLogQueue;
    };

}