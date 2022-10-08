module;
#include "cspch.hpp"
#include <entt/entt.hpp>
export module Editor.Panels;

import Cosmic.Base;
import Cosmic.App.Events;
import Cosmic.App.WindowEvents;

import Cosmic.App.Module;
import Cosmic.Renderer.Framebuffer;
import Cosmic.ECS.Scene;

namespace Cosmic
{

    export class Panel : public Module
    {
    public:
        Panel() = default;
        Panel(const String& name) : mPanelName(name) { }

    public:
        void  Show(bool open)         { mOpen = open;  }
        bool  IsOpen()          const { return mOpen;  }
        bool* IsOpenPtr()             { return &mOpen; }
        const String& GetPanelName() const { return mPanelName; }

    protected:
        bool mOpen = true;
        String mPanelName = "";
    };

    export class Panels
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