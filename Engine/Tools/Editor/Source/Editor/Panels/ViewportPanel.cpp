#include "cspch.hpp"
#include "Base/Macros.hpp"
#include "App/KeyAndMouseCodes.hpp"
#include "ECS/SceneCamera.hpp"
#include "Renderer/Framebuffer.hpp"
#include "SceneHierarchyPanel.hpp"
#include "App/Module.hpp"
#include "ViewportPanel.hpp"
#include "ContentBrowserPanel.hpp"
#include "App/File.hpp"
#include "Editor/EditorModule.hpp"
#include "ContentBrowserPanel.hpp"
#include "ECS/Components.hpp"

#include "Editor/Panels/PlaybarPanel.hpp"

#include "entt/entity/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <entt/entt.hpp>

CS_MODULE_LOG_INFO(Editor, Editor.Panel.ViewportPanel);

namespace Cosmic
{

    ViewportPanel::ViewportPanel()
        : IPanel("Viewport")
    {
        CS_PROFILE_FN();
    }

    void ViewportPanel::OnInit()
    {
        Ref<EditorModule> editorModule = ModuleSystem::Get<EditorModule>();

        FramebufferInfo fbInfo = {}; 
        fbInfo.Width           = 1280;
        fbInfo.Height          = 720;
        fbInfo.Samples         = 1;
        fbInfo.SwapChainTarget = false;
        fbInfo.AttachmentsInfo = FramebufferAttachmentsInfo({
            FramebufferTextureInfo(ETextureFormat::RGBA8_Float),
            FramebufferTextureInfo(ETextureFormat::R32_SInt)
        });

        mFramebuffer = CreateFramebuffer(fbInfo);

        mScene = editorModule->GetActiveScene();

        mSceneHierarchyPanel = ModuleSystem::Get<SceneHierarchyPanel>();
        mPlaybarPanel        = ModuleSystem::Get<PlaybarPanel>();
    }

    void ViewportPanel::OnUpdate(Dt dt)
    {
        //if (mWindowHovered)
        //    mCamera.OnUpdate();

        Renderer2D::ResetStatistics();

        mFramebuffer->Bind();
        RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
        RenderCommand::Clear();

        switch (mPlaybarPanel->GetSceneState())
        {
			case ESceneState::Edit:
			{
                mCamera.OnUpdate(dt);
                mScene->RenderCamera(mCamera, mCamera.GetTransform());
				break;
			}
			case ESceneState::Play:
			{
                mScene->RenderMainCamera();
				break;
			}
        }

        mFramebuffer->ClearAttachment(1, -1);

        auto [x, y] = ImGui::GetMousePos();
        x -= mTopLeft.x;
        y -= mTopLeft.y;
        y = mFramebuffer->GetInfo().Height - y;

        if (mWindowHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            int32 pixelData;
            pixelData = (int32)mFramebuffer->ReadPixel(1, { (int32)x, (int32)y });

            if (pixelData >= 0)
            {
                Entity entity = Entity((entt::entity)pixelData, mScene->GetRegistryPtr());
                mSceneHierarchyPanel->SetSelectedEntities({ entity });
            }
        }

        mFramebuffer->Unbind();
    }

    void ViewportPanel::OnEvent(const IEvent& e)
    {
        if (mWindowHovered)
            mCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(KeyPressEvent, OnKeyPressed);
        CS_DISPATCH_EVENT(SceneOpenedEvent, OnEditorSceneOpened);
        CS_DISPATCH_EVENT(ScenePlayEvent, OnScenePlay);
        CS_DISPATCH_EVENT(SceneStopEvent, OnSceneStop);
    }

    void ViewportPanel::OnImGuiRender()
    {
        CS_PROFILE_FN();

        if (!mOpen)
            return;

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoScrollbar;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin(mPanelName.c_str(), &mOpen, windowFlags);

        mWindowHovered = ImGui::IsWindowHovered();
        mWindowFocused = ImGui::IsWindowFocused();

        ImVec2 viewportMin = ImGui::GetWindowContentRegionMin();
        ImVec2 viewportMax = ImGui::GetWindowContentRegionMax();
        ImVec2 viewportPos = ImGui::GetWindowPos();
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        mTopLeft  = { viewportMin.x + viewportPos.x, viewportMin.y + viewportPos.y };
        mBtmRight = { viewportMax.x + viewportPos.x, viewportMax.y + viewportPos.y };

        // Block events if the panel is selected or hovered.
        Gui::BlockEvents(!mWindowHovered && !mWindowFocused);

        RenderResizing();
        RenderFramebuffer();
        RenderGizmo();
        RenderGrid();
        RenderDragDrop();

        ImGui::PopStyleVar(2);
        ImGui::End();
    }

    void ViewportPanel::RenderResizing()
    {
        uint32 fbWidth  = mFramebuffer->GetInfo().Width;
        uint32 fbHeight = mFramebuffer->GetInfo().Height;

        uint32 width  = ImGui::GetContentRegionAvail().x;
        uint32 height = ImGui::GetContentRegionAvail().y;

        // Check if the framebuffer's size matches the window size

        if (fbWidth != width || fbHeight != height && width > 0.0f && height > 0.0f || mSceneChanged)
        {
            mSceneChanged = false;

            mFramebuffer->Resize(width, height);
            mCamera.OnResized(width, height);

            mScene->OnViewportResize(width, height);
        }
    }

    void ViewportPanel::RenderFramebuffer()
    {
        uint32 textureID = mFramebuffer->GetColorAttachmentRendererID();
        ImGui::Image((ImTextureID)textureID, ImGui::GetContentRegionAvail(), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    }

    void ViewportPanel::RenderGizmo()
    {
        Vector<Entity> selectedEntities = mSceneHierarchyPanel->GetSelectedEntities();

        if (selectedEntities.size() == 1 && mGizmoOperation != (ImGuizmo::OPERATION)(-1))
        {
            Entity selectedEntity = selectedEntities[0];
            
            ImGuizmo::SetOrthographic(mCamera.GetProjectionType() == EProjectionType::Orthographic);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(mTopLeft.x, mTopLeft.y, mBtmRight.x - mTopLeft.x, mBtmRight.y - mTopLeft.y);

            auto& tc = selectedEntity.GetComponent<TransformComponent>();

            const glm::mat4& cameraProj = mCamera.GetProjection();
            const glm::mat4& cameraView = mCamera.GetTransform();
            glm::mat4        transform  = tc.GetAbsoluteTransform();

            bool snap = Input::IsKeyPressed(EKeyCode::LeftControl);

            float32 snapValue = mGizmoOperation == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;
            float32 snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(glm::inverse(cameraView)), glm::value_ptr(cameraProj), mGizmoOperation, mGizmoMode, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

                tc.Translation = translation;
                tc.Rotation = glm::radians(rotation);
                tc.Scale = scale;
            }
        }
    }

    void ViewportPanel::RenderGrid()
    {
        const glm::mat4& cameraView = mCamera.GetTransform();
        const glm::mat4& cameraProj = mCamera.GetProjection();
        static float32 theta = 0.0f;
        glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(theta * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
        theta++;

        //ImGuizmo::DrawGrid(glm::value_ptr(glm::inverse(cameraView)), glm::value_ptr(cameraProj), glm::value_ptr(transform), 100.0f);
    }

    void ViewportPanel::RenderDragDrop()
    {
        const String& contentItemDragDropStr = ModuleSystem::Get<ContentBrowserPanel>()->GetContentItemDragDropString();
                
        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(contentItemDragDropStr.c_str());

            if (payload)
            {
                String path = String((char*)payload->Data);
                path[payload->DataSize] = '\0';
                
                Ref<EditorModule> editorModule = ModuleSystem::Get<EditorModule>();
                editorModule->OpenScene(File(Path(path)));
            }
            
            ImGui::EndDragDropTarget();
        }
    }

    bool ViewportPanel::OnKeyPressed(const KeyPressEvent& e)
    {
        if (Input::IsKeyPressed(EKeyCode::LeftControl))
            return false;

        switch (e.GetKeyCode())
        {
            case EKeyCode::Q:
            {
                mGizmoOperation = (ImGuizmo::OPERATION)(-1);
                break;
            }
            case EKeyCode::W:
            {
                mGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                break;
            }
            case EKeyCode::E:
            {
                mGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                break;
            }
            case EKeyCode::R:
            {
                mGizmoOperation = ImGuizmo::OPERATION::SCALE;
                break;
            }
        }

        return false;
    }

    bool ViewportPanel::OnEditorSceneOpened(const SceneOpenedEvent& e)
    {
        mScene = e.GetScene();
        mSceneChanged = true;
        return true;
    }

    bool ViewportPanel::OnScenePlay(const ScenePlayEvent& e)
    {
        mScene = e.GetScene();

        return false;
    }

    bool ViewportPanel::OnSceneStop(const SceneStopEvent& e)
    {
        mScene = e.GetScene();

        return false;
    }

}
