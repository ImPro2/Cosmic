module;
#include "cspch.hpp"
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <iterator>
export module Cosmic.App.Module;

import Cosmic.Base;
import Cosmic.App.Event;

export import Cosmic.Base;
export import Cosmic.App.Event;

namespace Cosmic
{

    class Application;
    class ModuleSystem;

    // class to be inherited from
    export class Module
    {
    public:
        virtual void OnInit()     { };
        virtual void OnShutdown() { };
        virtual void OnUpdate()   { };
        virtual void OnEvent()    { };

    public:
        const String& GetName() const { return mName; }

    private:
        String mName;

    private:
        friend class ModuleSystem;
    };

    export class ModuleSystem
    {
    public:
        template<typename T>
        static void Add()
        {
            Scope<T> module = CreateScope<T>();
            module->mName = typeid(T).name();
            module->OnInit();
            sModules.push_back(module);
        }

        template<typename T>
        static void Remove()
        {
            std::erase_if(sModules, [](Scope<Module> module)
            {
                if (module->mName == typeid(T).name())
                {
                    module->OnShutdown();
                    return true;
                }

                return false;
            });
        }

    private:
        static void Init();
        static void Shutdown();

        static void OnUpdate();
        static void OnEvent(const WindowEvent& e);

    private:
        inline static std::vector<Scope<Module>> sModules;
        friend class Application;
    };

}