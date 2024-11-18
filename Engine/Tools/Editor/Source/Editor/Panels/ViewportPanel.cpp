#include "cspch.hpp"
#include "App/Module.hpp"
#include "ViewportPanel.hpp"
#include "ContentBrowserPanel.hpp"
#include "App/File.hpp"
#include "Editor/EditorModule.hpp"

#include <imgui.h>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace Cosmic
{

    ViewportPanel::ViewportPanel(const Ref<Framebuffer>& framebuffer, const Ref<Scene>& scene, ContentBrowserPanel* contentBrowserPanel)
        : mFramebuffer(framebuffer), mScene(scene), mContentBrowserPanel(contentBrowserPanel), Panel("Viewport")
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

        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(EditorSceneOpenedEvent, OnEditorSceneOpened);
    }

    void ViewportPanel::OnImGuiRender()
    {
        CS_PROFILE_FN();

        if (!mOpen)
            return;

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
        windowFlags |= ImGuiWindowFlags_NoNavInputs;
        windowFlags |= ImGuiWindowFlags_NoScrollbar;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin(mPanelName.c_str(), &mOpen, windowFlags);
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

            if (fbWidth != width || fbHeight != height && width > 0.0f && height > 0.0f || mSceneChanged)
            {
                mSceneChanged = false;

                mFramebuffer->Resize(width, height);
                mCameraController.OnResize(width, height);

                mScene->OnViewportResize(width, height);
            }

            // Render the image

            uint32 textureID = mFramebuffer->GetColorAttachmentRendererID();
            ImGui::Image((void*)textureID, ImVec2((float32)width, (float32)height), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

            const String& contentItemDragDropStr = mContentBrowserPanel->GetContentItemDragDropString();
            
            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(contentItemDragDropStr.c_str());

                if (payload)
                {
                    File sceneFile(Path((char*)payload->Data));
                    
                    EditorModule* editorModule = ModuleSystem::Get<EditorModule>();
                    editorModule->OpenScene(sceneFile);
                }
                
                ImGui::EndDragDropTarget();
            }
        }

        ImGui::PopStyleVar(2);
        ImGui::End();
    }

    bool ViewportPanel::OnEditorSceneOpened(const EditorSceneOpenedEvent& e)
    {
        mScene = e.GetScene();
        mSceneChanged = true;
        return true;
    }

}
