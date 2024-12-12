#pragma once
#include <entt/entt.hpp>

#include "Base/Base.hpp"
#include "App/Module.hpp"
#include "Renderer/Framebuffer.hpp"
#include "ECS/Scene.hpp"

namespace Cosmic
{

    class IPanel : public IModule
    {
    public:
        IPanel() = default;
        IPanel(const String& name) : mPanelName(name) { }

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
        void Init();
        void ShowAll();

    public:
        template<typename T>
        Ref<T> GetPanel()
        {
            const char* name = typeid(T).name();
            for (Ref<IPanel> panel : mPanels)
            {
                if (panel->GetName() == name)
                    return panel.As<T>();
            }
        }

        const Vector<Ref<IPanel>>& GetPanels() const { return mPanels; }

    private:
        Vector<Ref<IPanel>> mPanels;
    };

}
