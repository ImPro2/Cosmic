#pragma once
#include <entt/entt.hpp>

#include "Panels.hpp"

#include "App/Module.hpp"
#include "Base/Base.hpp"
#include "Renderer/Framebuffer.hpp"
#include "Renderer/OrthographicCameraController.hpp"
#include "Time/DeltaTime.hpp"
#include "Gui/Gui.hpp"
#include "App/Event/Events.hpp"
#include "ECS/Scene.hpp"

#include "App/Event/WindowEvents.hpp"
#include "Editor/Event/EditorSceneEvents.hpp"
#include "Editor/EditorCamera.hpp"

#include "SceneHierarchyPanel.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#define USE_IMGUI_API
#include <ImGuizmo.h>

namespace Cosmic
{

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
        bool OnEditorSceneOpened(const EditorSceneOpenedEvent& e);

    private:
        Ref<Framebuffer> mFramebuffer;
        Ref<Scene>       mScene;
        bool mSceneChanged = false;

        bool mWindowHovered = false;
        bool mWindowFocused = false;

        ImVec2 mTopLeft, mBtmRight;

        EditorCamera mCamera;

        Ref<SceneHierarchyPanel> mSceneHierarchyPanel;

        ImGuizmo::OPERATION mGizmoOperation = (ImGuizmo::OPERATION)(-1);
        ImGuizmo::MODE      mGizmoMode = ImGuizmo::MODE::LOCAL;
    };

}
