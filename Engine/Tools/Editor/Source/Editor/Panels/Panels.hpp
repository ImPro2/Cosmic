#pragma once
#include <entt/entt.hpp>

#include "Base/Base.hpp"
#include "App/Module.hpp"
#include "Renderer/Framebuffer.hpp"
#include "ECS/Scene.hpp"
#include "UI/Layout/Layout.hpp"

namespace Cosmic
{

    class IPanel : public IModule
    {
    public:
        IPanel() = default;
        IPanel(const String& name) : mPanelName(name) { }

    public:
        void  Show(bool open)              { mOpen = open;       }
        bool  IsOpen()               const { return mOpen;       }
        bool* IsOpenPtr()                  { return &mOpen;      }
        const String& GetPanelName() const { return mPanelName;  }

        DockNodeID GetDockNodeID()   const { return mDockNodeID; }

    public:
        void SetDockNodeID(DockNodeID id)  { mDockNodeID = id;   }

    protected:
        bool       mOpen      = true;
        String     mPanelName = "";
        DockNodeID mDockNodeID;
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

        Vector<Ref<IPanel>> GetOpenPanels() const
        {
            Vector<Ref<IPanel>> panels;
            panels.reserve(mPanels.size());

            for (Ref<IPanel> panel : mPanels)
            {
                if (panel->IsOpen())
                    panels.push_back(panel);
            }

            return panels;
        }

        Ref<IPanel> FindPanelByName(const String& name) const
        {
            for (Ref<IPanel> panel : mPanels)
            {
                if (name == panel->GetPanelName())
                    return panel;
            }

            return Ref<IPanel>(nullptr);
        }

        const Vector<Ref<IPanel>>& GetPanels() const { return mPanels; }

    private:
        Vector<Ref<IPanel>> mPanels;
    };

}
