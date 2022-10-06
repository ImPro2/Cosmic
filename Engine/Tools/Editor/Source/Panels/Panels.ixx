module;
#include "cspch.hpp"
export module Editor.Panels;

import Cosmic.Base;
import Cosmic.App.Events;
import Cosmic.App.WindowEvents;

import Cosmic.App.Module;
import Cosmic.Renderer.Framebuffer;

namespace Cosmic
{

    export class Panel : public Module
    {
    public:
        void Show(bool open) { mOpen = open; }
        bool IsOpen()  const { return mOpen; }

    protected:
        bool mOpen = true;
    };

    export class Panels
    {
    public:
        void Init(const Ref<Framebuffer>& framebuffer);
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

    private:
        Vector<Panel*> mPanels;
    };

}