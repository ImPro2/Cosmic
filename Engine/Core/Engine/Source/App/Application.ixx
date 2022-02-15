module;
#include "cspch.hpp"
export module Cosmic.App.Application;

import Cosmic.Base.Types;
import Cosmic.Base.Tuples;
import Cosmic.App.IWindow;

namespace Cosmic
{

    export struct ApplicationInfo
    {
        const char*       Name;
        DesktopWindowInfo WindowInfo;
    };

    export class Application
    {
    public:
        Application();
        ~Application();

        void Close();

    protected:
        void Init(ApplicationInfo&& info);

    private:
        void Run();

    public:
        static Application& Get() { return *sInstance; }

    private:
        inline static Application* sInstance = nullptr;
        ApplicationInfo mInfo;
        Scope<IDesktopWindow> mWindow;
    };

}