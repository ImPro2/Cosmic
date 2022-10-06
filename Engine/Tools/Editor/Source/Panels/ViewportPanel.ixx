module;
#include "cspch.hpp"
export module Editor.Panels.ViewportPanel;

import Editor.Panels;
import Cosmic.App.Module;
import Cosmic.Base;
import Cosmic.Renderer.Framebuffer;
import Cosmic.Renderer.OrthographicCameraController;
import Cosmic.Time.DeltaTime;
import Cosmic.Gui;
import Cosmic.App.Events;

namespace Cosmic
{

    export class ViewportPanel : public Panel
    {
    public:
        ViewportPanel(const Ref<Framebuffer>& framebuffer);

    public:
        void OnUpdate(Dt dt) override;
        void OnEvent(const Event& e) override;
        void OnImGuiRender() override;

    public:
        const OrthographicCameraController& GetCameraController() const { return mCameraController; }

    private:
        Ref<Framebuffer> mFramebuffer;
        OrthographicCameraController mCameraController;
    };

}