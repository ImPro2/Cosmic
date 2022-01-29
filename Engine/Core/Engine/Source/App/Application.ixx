module;
#include "cspch.hpp"
export module Cosmic.App.Application;

namespace Cosmic
{

    export struct ApplicationInfo
    {
        const char* Name;
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
    };

}