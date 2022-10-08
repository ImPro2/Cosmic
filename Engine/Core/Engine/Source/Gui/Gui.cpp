module;
#include "cspch.hpp"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <IconsFontAwesome6.h>
module Cosmic.Gui;
CS_MODULE_LOG_INFO(Cosmic, Gui);

import Cosmic.App.Application;
import Cosmic.App.IWindow;
import Cosmic.App.Events;

namespace Cosmic
{

    static bool sInitialized = false;

    static void SetDarkThemeColors()
    {
        constexpr auto colorFromBytes = [](const uint8_t r, const uint8_t g, const uint8_t b)
        {
            return ImVec4(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, 1.0f);
        };

        auto& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        style.TabRounding = 3.5f;
        style.FrameRounding = 3.5f;
        //style.FrameBorderSize = 1.0f;
        style.PopupRounding = 3.5f;
        style.ScrollbarRounding = 3.5f;
        style.GrabRounding = 3.5f;
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.DisplaySafeAreaPadding = ImVec2(0, 0);

        // Headers
        colors[ImGuiCol_Header] = colorFromBytes(62, 62, 62);
        colors[ImGuiCol_HeaderHovered] = colorFromBytes(56, 56, 56);

        // Checbox
        colors[ImGuiCol_CheckMark] = colorFromBytes(255, 255, 255);

        // Buttons
        colors[ImGuiCol_Button] = colorFromBytes(25, 25, 25);
        colors[ImGuiCol_ButtonHovered] = colorFromBytes(110, 110, 110);
        colors[ImGuiCol_ButtonActive] = colorFromBytes(120, 120, 120);

        // Frame
        colors[ImGuiCol_FrameBg] = colorFromBytes(25, 25, 25);
        colors[ImGuiCol_FrameBgHovered] = colorFromBytes(88, 88, 88);
        colors[ImGuiCol_FrameBgActive] = colorFromBytes(110, 110, 110);

        // Tabs
        colors[ImGuiCol_Tab] = colorFromBytes(56, 56, 56);
        colors[ImGuiCol_TabHovered] = colorFromBytes(56, 56, 56);
        colors[ImGuiCol_TabActive] = colorFromBytes(90, 90, 90);
        colors[ImGuiCol_TabUnfocused] = colorFromBytes(40, 40, 40);
        colors[ImGuiCol_TabUnfocusedActive] = colorFromBytes(88, 88, 88);

        // Title
        colors[ImGuiCol_TitleBg] = colorFromBytes(40, 40, 40);
        colors[ImGuiCol_TitleBgActive] = colorFromBytes(40, 40, 40);

        // Others
        colors[ImGuiCol_WindowBg] = colorFromBytes(45, 45, 45);
        colors[ImGuiCol_PopupBg] = colorFromBytes(45, 45, 45);
        colors[ImGuiCol_DockingPreview] = colorFromBytes(26, 26, 26);

        //colors[ImGuiCol_Separator] = colorFromBytes(10, 200, 10);
        //colors[ImGuiCol_Border] = colorFromBytes(10, 200, 10);
    }

    void Gui::Init()
    {
        CS_PROFILE_FN();
    
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

        ImFontConfig config;
        config.MergeMode = true;
        static const ImWchar iconRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        io.FontDefault = io.Fonts->AddFontFromFileTTF("C:/dev/Cosmic/Engine/Core/Engine/Assets/Fonts/Ruda-Bold.ttf", 18.0f);
        io.Fonts->AddFontFromFileTTF("C:/dev/Cosmic/Engine/Core/Engine/Assets/Fonts/fontawesome-webfont.ttf", 18.0f, &config, iconRanges);

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColors();

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