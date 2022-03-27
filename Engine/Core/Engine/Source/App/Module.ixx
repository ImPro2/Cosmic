module;
#include "cspch.hpp"
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <iterator>
export module Cosmic.App.Module;

import Cosmic.Base;
import Cosmic.App.Events;

export import Cosmic.Base;
export import Cosmic.App.Events;

namespace Cosmic
{

    class Application;
    class ModuleSystem;

    // class to be inherited from
    export class Module
    {
    public:
        virtual void OnInit()                      { };
        virtual void OnShutdown()                  { };
        virtual void OnUpdate()                    { };
        virtual void OnEvent(const WindowEvent& e) { };

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
            Module* module = new T();
            module->mName = typeid(T).name();
            module->OnInit();
            sModules.push_back(module);
        }

        template<typename T>
        static void Remove()
        {
            std::erase_if(sModules, [](Module* module)
            {
                if (module->mName == typeid(T).name())
                {
                    module->OnShutdown();
                    delete module;
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
        inline static std::vector<Module*> sModules;
        friend class Application;
    };

}