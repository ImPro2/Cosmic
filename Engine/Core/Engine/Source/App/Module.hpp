#pragma once
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <iterator>
#include "Base/Base.hpp"
#include "App/Event/Events.hpp"
#include "Time/Time.hpp"
#include "Time/DeltaTime.hpp"

#include "Base/Base.hpp"
#include "App/Event/WindowEvents.hpp"
#include "App/Event/Events.hpp"

namespace Cosmic
{

    class Application;
    class ModuleSystem;

    // class to be inherited from
    class Module
    {
    public:
        virtual void OnInit()                { };
        virtual void OnShutdown()            { };
        virtual void OnUpdate(Dt dt)         { };
        virtual void OnEvent(const Event& e) { };
        virtual void OnImGuiRender()         { };

    public:
        const String& GetName() const { return mName; }

    private:
        String mName;

    private:
        friend class ModuleSystem;
    };

    class ModuleSystem
    {
    public:
        template<typename T, typename ... Args>
        static void Add(Args&& ... args)
        {
            Module* module = new T(std::forward<Args>(args)...);
            module->mName = typeid(T).name();
            module->OnInit();
            sModules.push_back(module);
        }

        template<typename T, typename ... Args>
        static void AddFront(Args&& ... args)
        {
            Module* module = new T(std::forward<Args>(args)...);
            module->mName = typeid(T).name();
            module->OnInit();
            sFrontModules.push_back(module);
        }

        template<typename T>
        static void Remove()
        {
            auto eraseFunction = [](Module* module)
            {
                if (module->mName == typeid(T).name())
                {
                    module->OnShutdown();
                    delete module;
                    return true;
                }

                return false;
            };

            std::erase_if(sFrontModules, eraseFunction);
            std::erase_if(sModules, eraseFunction);
        }

        template<typename T>
        static T* Get()
        {
            const char* name = typeid(T).name();
            for (Module* module : sFrontModules)
            {
                if (module->GetName() == name)
                    return static_cast<T*>(module);
            }
            for (Module* module : sModules)
            {
                if (module->GetName() == name)
                    return static_cast<T*>(module);
            }
        }

    private:
        static void Init();
        static void Shutdown();

        static void OnUpdate();
        static void OnEvent(const Event& e);
        static void OnImGuiRender();

    private:
        inline static std::vector<Module*> sModules;
        inline static std::vector<Module*> sFrontModules;
        friend class Application;
    };

}