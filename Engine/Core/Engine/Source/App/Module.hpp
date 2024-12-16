#pragma once
#include <memory>
#include <queue>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <iterator>
#include "Base/Base.hpp"
#include "Time/Time.hpp"
#include "Time/DeltaTime.hpp"
#include "Memory/Memory.hpp"

#include "Event/Events.hpp"
#include "Event/Type/WindowEvents.hpp"

namespace Cosmic
{

    class Application;
    class ModuleSystem;

    // class to be inherited from
    class IModule : public IRefCounted
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

    class ModuleSystem : public IRefCounted
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
            sInstance->mModules.push_back(module);
            module->mName = typeid(T).name();
            module->OnInit();

            return module.As<T>();
        }

        template<typename T, typename ... Args>
        static Ref<T> AddFront(Args&& ... args)
        {
            if (Get<T>())
                return Get<T>();

            Ref<IModule> module = CreateRef<T>(std::forward<Args>(args)...);
            sInstance->mModules.insert(sInstance->mModules.begin(), module);
            module->mName = typeid(T).name();
            module->OnInit();

            return module.As<T>();
        }

        template<typename T, typename ... Args>
        static Ref<T> AddDeferred(Args&& ... args)
        {
            if (Get<T>())
                return Get<T>();

            Ref<IModule> module = CreateRef<T>(std::forward<Args>(args)...);
            module->mName = typeid(T).name();
            sInstance->mDeferredAddModules.push({ module, EDeferredInsertMode::Back });

            return module.As<T>();
        }

        template<typename T, typename ... Args>
        static Ref<T> AddFrontDeferred(Args&& ... args)
        {
            if (Get<T>())
                return Get<T>();

            Ref<IModule> module = CreateRef<T>(std::forward<Args>(args)...);
            module->mName = typeid(T).name();
            sInstance->mDeferredAddModules.push({ module, EDeferredInsertMode::Front });

            return module.As<T>();
        }

        template<typename T>
        static void Remove()
        {
            auto eraseFunction = [](Ref<IModule> module)
            {
                if (module->mName == typeid(T).name())
                {
                    module->OnShutdown();
                    module.Release();
                    return true;
                }

                return false;
            };

            std::erase_if(sInstance->mModules, eraseFunction);
        }

        template<typename T>
        static void RemoveDeferred()
        {
            sInstance->mDeferredRemoveModules.push(Get<T>());
        }

        template<typename T>
        static Ref<T> Get()
        {
            const char* name = typeid(T).name();

            for (Ref<IModule> module : sInstance->mModules)
            {
                if (module->GetName() == name)
                    return module.As<T>();
            }

            return Ref<T>(nullptr);
        }

    private:
        static Ref<ModuleSystem> Init();
        static void              Shutdown();

        static void OnUpdate();
        static void OnEvent(const IEvent& e);
        static void OnImGuiRender();

        static void AddDeferredModules();
        static void RemoveDeferredModules();

    private:
        Vector<Ref<IModule>> mModules;

        std::queue<Pair<Ref<IModule>, EDeferredInsertMode>> mDeferredAddModules;
        std::queue<Ref<IModule>>                            mDeferredRemoveModules;

        inline static Ref<ModuleSystem> sInstance;

        friend class Application;
    };

}
