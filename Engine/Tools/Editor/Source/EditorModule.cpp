module;
#include "cspch.hpp"
#include <imgui.h>
module Editor.EditorModule;

CS_MODULE_LOG_INFO(Editor, EditorModule);


namespace Cosmic
{

    void EditorModule::OnInit()
    {
        CS_PROFILE_FN();

        mCosmicLogoTexture = CreateTexture2D("C:/dev/Cosmic/Branding/Logos/Logo.png");

        FramebufferInfo fbInfo = {};
        fbInfo.Width = 1280;
        fbInfo.Height = 720;
        fbInfo.SwapChainTarget = false;
        mFramebuffer = CreateFramebuffer(fbInfo);

        mPanels.Init(mFramebuffer);

        CS_LOG_INFO("Successfully initialized editor.");
    }

    void EditorModule::OnShutdown()
    {
        CS_PROFILE_FN();

        CS_LOG_INFO("Sucessfully shut editor down.");
    }

    void EditorModule::OnUpdate(Dt dt)
    {
        CS_PROFILE_FN();

        mFramebuffer->Bind();
        RenderCommand::Clear();
        RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

        const OrthographicCamera& camera = mPanels.GetPanel<ViewportPanel>()->GetCameraController().GetCamera();

        // Temporary
        Renderer2D::BeginScene(camera);
        {
            Renderer2D::RenderQuad({ 0.0f, 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, mCosmicLogoTexture);
        }
        Renderer2D::EndScene();

        mFramebuffer->Unbind();

        CS_LOG_INFO("FPS: {}s, Delta Time: {}ms, Current Time: {}s", Time::GetFPS().InSeconds(), Time::GetDeltaTime().InMilliSeconds(), Time::GetTime().InSeconds());
    }

    void EditorModule::OnEvent(const Event& e)
    {
        CS_PROFILE_FN();
    }

    void EditorModule::OnImGuiRender()
    {
        CS_PROFILE_FN();

        SetupDockSpace();

        ImGui::ShowDemoWindow();
    }

    void EditorModule::SetupDockSpace()
    {
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        static bool open = true;
        ImGui::Begin("Editor Dockspace", &open, windowFlags);

        ImGui::PopStyleVar(3);

        ImGuiIO& io = ImGui::GetIO();
        ImGuiID dockspaceID = ImGui::GetID("Editor Dockspace");
        ImGui::DockSpace(dockspaceID);

        ImGui::End();
    }

}