#pragma once
#include <queue>

#include "Panels.hpp"

#include "App/Module.hpp"
#include "Base/Base.hpp"
#include "App/Log/Log.hpp"
#include "App/Log/ConsoleColor.hpp"
#include "App/Log/LogSeverity.hpp"
#include "App/Event/LogEvents.hpp"
#include "Base/StringUtils.hpp"

namespace Cosmic
{

    class ConsolePanel : public Panel
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