#pragma once
#include <memory>
#include <queue>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <iterator>
#include "Base/Base.hpp"
#include "App/Event/Events.hpp"
#include "Time/Time.hpp"
#include "Time/DeltaTime.hpp"

#include "App/Event/WindowEvents.hpp"
#include "App/Event/Events.hpp"

namespace Cosmic
{

    class Application;
    class ModuleSystem;

    // class to be inherited from
    class IModule
    {
    public:
        virtual void OnInit()                 { };
        virtual void OnShutdown()             { };
        virtual void OnUpdate(Dt dt)          { };
        virtual void OnEvent(const IEvent& e) { };
        virtual void OnImGuiRender()          { };

    public:
        const String& GetName() const { return mName; }

    private:
        String mName;

    private:
        friend class ModuleSystem;
    };

    class ModuleSystem
    {
    private:
        enum class EDeferredInsertMode
        {
            Front, Back
        };

    public:
        template<typename T, typename... Args>
        static Ref<T> Add(Args&&... args)
        {
            if (Get<T>())
                return Get<T>();

            Ref<IModule> module = CreateRef<T>(std::forward<Args>(args)...);
            sModules.push_back(module);
            module->mName = typeid(T).name();
            module->OnInit();

            return std::static_pointer_cast<T>(module);
        }

        template<typename T, typename ... Args>
        static Ref<T> AddFront(Args&& ... args)
        {
            if (Get<T>())
                return Get<T>();

            Ref<IModule> module = CreateRef<T>(std::forward<Args>(args)...);
            sModules.insert(sModules.begin(), module);
            module->mName = typeid(T).name();
            module->OnInit();

            return std::static_pointer_cast<T>(module);
        }

        template<typename T, typename ... Args>
        static Ref<T> AddDeferred(Args&& ... args)
        {
            if (Get<T>().get())
                return Get<T>();

            Ref<IModule> module = CreateRef<T>(std::forward<Args>(args)...);
            module->mName = typeid(T).name();
            sDeferredModules.push({ module, EDeferredInsertMode::Back });

            return std::static_pointer_cast<T>(module);
        }

        template<typename T, typename ... Args>
        static Ref<T> AddFrontDeferred(Args&& ... args)
        {
            if (Get<T>().get())
                return Get<T>();

            Ref<IModule> module = CreateRef<T>(std::forward<Args>(args)...);
            module->mName = typeid(T).name();
            sDeferredModules.push({ module, EDeferredInsertMode::Front });

            return std::static_pointer_cast<T>(module);
        }

        template<typename T>
        static void Remove()
        {
            auto eraseFunction = [](Ref<IModule> module)
            {
                if (module->mName == typeid(T).name())
                {
                    module->OnShutdown();
                    module.reset();
                    return true;
                }

                return false;
            };

            std::erase_if(sModules, eraseFunction);
        }

        template<typename T>
        static Ref<T> Get()
        {
            const char* name = typeid(T).name();

            for (Ref<IModule> module : sModules)
            {
                if (module->GetName() == name)
                    return std::static_pointer_cast<T>(module);
            }

            return Ref<T>(nullptr);
        }

    private:
        static void Init();
        static void Shutdown();

        static void OnUpdate();
        static void OnEvent(const IEvent& e);
        static void OnImGuiRender();

        static void AddDeferredModules();

    private:
        inline static Vector<Ref<IModule>> sModules;
        inline static std::queue<Pair<Ref<IModule>, EDeferredInsertMode>> sDeferredModules;
        friend class Application;
    };

}
