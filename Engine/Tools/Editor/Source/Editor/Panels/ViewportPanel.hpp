#pragma once
#include <entt/entt.hpp>

#include "Panels.hpp"

#include "App/Module.hpp"
#include "Base/Base.hpp"
#include "Renderer/Framebuffer.hpp"
#include "Renderer/OrthographicCameraController.hpp"
#include "Time/DeltaTime.hpp"
#include "Gui/Gui.hpp"
#include "ECS/Scene.hpp"

#include "Event/Type/WindowEvents.hpp"
#include "Editor/Event/SceneEvents.hpp"
#include "Editor/EditorCamera.hpp"

#include "Editor/Panels/SceneHierarchyPanel.hpp"
#include "Editor/Panels/PlaybarPanel.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#define USE_IMGUI_API
#include <ImGuizmo.h>

namespace Cosmic
{

    class PlaybarPanel;

    class ViewportPanel : public IPanel
    {
    public:
        ViewportPanel();

    public:
        void OnInit()                 override;
        void OnUpdate(Dt dt)          override;
        void OnEvent(const IEvent& e) override;
        void OnImGuiRender()          override;

    private:
        void RenderResizing();
        void RenderFramebuffer();
        void RenderGizmo();
        void RenderGrid();
        void RenderDragDrop();

    private:
        bool OnKeyPressed(const KeyPressEvent& e);
        bool OnEditorSceneOpened(const SceneOpenedEvent& e);
        bool OnScenePlay(const ScenePlayEvent& e);
        bool OnSceneStop(const SceneStopEvent& e);

    public:
        const EditorCamera& GetEditorCamera() const { return mCamera; }

    private:
        Ref<Framebuffer> mFramebuffer;
        Ref<Scene>       mScene;

        bool mSceneChanged = false;

        bool mWindowHovered = false;
        bool mWindowFocused = false;

        ImVec2 mTopLeft, mBtmRight;

        EditorCamera mCamera;

        Ref<SceneHierarchyPanel> mSceneHierarchyPanel;
        Ref<PlaybarPanel>        mPlaybarPanel;

        ImGuizmo::OPERATION mGizmoOperation = (ImGuizmo::OPERATION)(-1);
        ImGuizmo::MODE      mGizmoMode      = ImGuizmo::MODE::LOCAL;
    };

}
