module;
#include "cspch.hpp"
#include <imgui.h>
#include <glm/glm.hpp>
module Editor.Panels.ViewportPanel;

namespace Cosmic
{

    ViewportPanel::ViewportPanel(const Ref<Framebuffer>& framebuffer)
        : mFramebuffer(framebuffer)
    {
        CS_PROFILE_FN();

        mCameraController.SetRotation(false);
    }

    void ViewportPanel::OnUpdate(Dt dt)
    {
        mCameraController.OnUpdate();
    }

    void ViewportPanel::OnEvent(const Event& e)
    {
        mCameraController.OnEvent(e);
    }

    void ViewportPanel::OnImGuiRender()
    {
        CS_PROFILE_FN();

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
        windowFlags |= ImGuiWindowFlags_NoNavInputs;
        windowFlags |= ImGuiWindowFlags_NoScrollbar;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("Viewport", &mOpen, windowFlags);
        {
            // BLock events if the panel is selected or hovered.
            bool hovered = ImGui::IsWindowHovered();
            bool focused = ImGui::IsWindowFocused();
            Gui::BlockEvents(!hovered && !focused);

            uint32 fbWidth  = mFramebuffer->GetInfo().Width;
            uint32 fbHeight = mFramebuffer->GetInfo().Height;

            uint32 width  = ImGui::GetContentRegionAvail().x;
            uint32 height = ImGui::GetContentRegionAvail().y;

            // Check if the framebuffer's size matches the window size

            if (fbWidth != width || fbHeight != height)
            {
                mFramebuffer->Resize(width, height);
                mCameraController.OnResize(width, height);
            }

            // Render the image

            uint32 textureID = mFramebuffer->GetColorAttachmentRendererID();
            ImGui::Image((void*)textureID, ImVec2((float32)width, (float32)height), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
        }

        ImGui::PopStyleVar(2);
        ImGui::End();
    }

}