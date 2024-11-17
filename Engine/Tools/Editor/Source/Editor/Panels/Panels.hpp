#pragma once
#include <entt/entt.hpp>

#include "Base/Base.hpp"
#include "App/Event/Events.hpp"
#include "App/Event/WindowEvents.hpp"
#include "App/Module.hpp"
#include "Renderer/Framebuffer.hpp"
#include "ECS/Scene.hpp"

namespace Cosmic
{

    class Panel : public Module
    {
    public:
        Panel() = default;
        Panel(const String& name) : mPanelName(name) { }

    public:
        void  Show(bool open)              { mOpen = open;      }
        bool  IsOpen()               const { return mOpen;      }
        bool* IsOpenPtr()                  { return &mOpen;     }
        const String& GetPanelName() const { return mPanelName; }

    protected:
        bool mOpen = true;
        String mPanelName = "";
    };

    class Panels
    {
    public:
        void Init(const Ref<Framebuffer>& framebuffer, const Ref<Scene>& scene);
        void ShowAll();

    public:
        template<typename T>
        T* GetPanel()
        {
            const char* name = typeid(T).name();
            for (Panel* panel : mPanels)
            {
                if (panel->GetName() == name)
                    return static_cast<T*>(panel);
            }
        }

        const Vector<Panel*>& GetPanels() const { return mPanels; }

    private:
        Vector<Panel*> mPanels;
    };

}
