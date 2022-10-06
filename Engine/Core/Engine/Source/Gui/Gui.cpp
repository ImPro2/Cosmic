module;
#include "cspch.hpp"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
module Cosmic.Gui;
CS_MODULE_LOG_INFO(Cosmic, Gui);

import Cosmic.App.Application;
import Cosmic.App.IWindow;
import Cosmic.App.Events;

namespace Cosmic
{

    static bool sInitialized = false;

    void Gui::Init()
    {
        CS_PROFILE_FN();
    
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

        float fontsize = 18.0f;
        io.Fonts->AddFontFromFileTTF("C:/dev/Cosmic/Engine/Core/Engine/Assets/Fonts/OpenSans-Bold.ttf", fontsize);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("C:/dev/Cosmic/Engine/Core/Engine/Assets/Fonts/OpenSans-Regular.ttf", 18);

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        //GLFWwindow* windowHandle = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetHandle());
        GLFWwindow* windowHandle = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetHandle());
    
        ImGui_ImplGlfw_InitForOpenGL(windowHandle, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        sInitialized = true;
    }

    void Gui::Shutdown()
    {
        CS_PROFILE_FN();
    
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Gui::OnEvent(Event* e)
    {
        CS_PROFILE_FN();
        if (!sInitialized) return;
    
        if (sBlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();

            bool mouse = (e->GetType() == EEventType::MouseMove) || (e->GetType() == EEventType::MouseScroll) || (e->GetType() == EEventType::MouseButtonClick);
            bool keyboard = (e->GetType() == EEventType::KeyPress) || (e->GetType() == EEventType::KeyRelease) || (e->GetType() == EEventType::KeyType);

            e->Block |= mouse & io.WantCaptureMouse;
            e->Block |= keyboard & io.WantCaptureKeyboard;
        }
    }

    void Gui::Begin()
    {
        CS_PROFILE_FN();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Gui::End()
    {
        CS_PROFILE_FN();

        ImGuiIO&        io         = ImGui::GetIO();
        //Application& app = Application::Get();
        //IDesktopWindow* window = app.GetWindow();
        //io.DisplaySize = ImVec2((float32)window->GetSize().x, (float32)window->GetSize().y);
    
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

}