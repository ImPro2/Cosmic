#include "cspch.hpp"
#include "Layout.hpp"
#include "Editor/EditorModule.hpp"
#include "Editor/Panels/Panels.hpp"
#include "UI/Layout/LayoutManager.hpp"
#include "UI/DockspaceModule.hpp"

#include "Editor/Panels/Panels.hpp"
#include "Editor/Panels/ConsolePanel.hpp"
#include "Editor/Panels/ViewportPanel.hpp"
#include "Editor/Panels/InspectorPanel.hpp"
#include "Editor/Panels/SceneHierarchyPanel.hpp"
#include "Editor/Panels/ContentBrowserPanel.hpp"

#include "Memory/PersistentStackAllocator.hpp"

#include <imgui.h>

namespace Cosmic
{

    namespace Utils
    {

        static ImGuiDir EDockSplitDirToImGuiDir(EDockSplitDir dir)
        {
            switch (dir)
            {
				case EDockSplitDir::Down:  return ImGuiDir_Down;
				case EDockSplitDir::Left:  return ImGuiDir_Left;
				case EDockSplitDir::Up:    return ImGuiDir_Up;
				case EDockSplitDir::Right: return ImGuiDir_Right;
				case EDockSplitDir::Stack: return ImGuiDir_None;
            }
        }

        template<typename... Args>
        static DockNode* AllocateDockNode(Args&&... args)
        {
            return PersistentStackAllocator::Allocate<DockNode>(std::forward<Args>(args)...);
        }

        // Breadth-first
        template<typename Callback>
        static void IterateDockNode(DockNode* node, Callback fn)
        {
            fn(node);

            if (node->Child1)
                IterateDockNode(node->Child1, fn);
            if (node->Child2)
                IterateDockNode(node->Child2, fn);
        }

    }

	DockNode::DockNode(const WeakRef<IPanel>& panel)
		: Panel(panel)
	{
	}

    DockNode* DockNode::Split(EDockSplitDir dir, float32 splitPercent, DockNode* child1, DockNode* child2)
    {
        DockNode* node = Utils::AllocateDockNode();

        node->Child1       = child1;
        node->Child2       = child2;
        node->SplitDir     = dir;
        node->SplitPercent = splitPercent;

        return node;
    }

    DockNode* DockNode::Split(EDockSplitDir dir, float32 splitPercent, DockNode* child1, DockNode&& child2)
    {
        DockNode* child2Ptr = Utils::AllocateDockNode(std::move(child2));

        Split(dir, splitPercent, child1, child2Ptr);
    }

    DockNode* DockNode::Split(EDockSplitDir dir, float32 splitPercent, DockNode&& child1, DockNode* child2)
    {
        DockNode* child1Ptr = Utils::AllocateDockNode(std::move(child1));

        Split(dir, splitPercent, child1Ptr, child2);
    }

    DockNode* DockNode::Split(EDockSplitDir dir, float32 splitPercent, DockNode&& child1, DockNode&& child2)
    {
        DockNode* child1Ptr = Utils::AllocateDockNode(std::move(child1));
        DockNode* child2Ptr = Utils::AllocateDockNode(std::move(child2));

        Split(dir, splitPercent, child1Ptr, child2Ptr);
    }

    DockNode* DockNode::Stack(DockNode* child1, DockNode* child2)
    {
        DockNode* node = Utils::AllocateDockNode();

        node->SplitDir = EDockSplitDir::Stack;
        node->Child1   = child1;
        node->Child2   = child2;

        return node;
    }

    DockNode* DockNode::Stack(DockNode* child1, DockNode&& child2)
    {
        DockNode* child2Ptr = Utils::AllocateDockNode(std::move(child2));

        Stack(child1, child2Ptr);
    }

    DockNode* DockNode::Stack(DockNode&& child1, DockNode* child2)
    {
        DockNode* child1Ptr = Utils::AllocateDockNode(std::move(child1));

        Stack(child1Ptr, child2);
    }

    DockNode* DockNode::Stack(DockNode&& child1, DockNode&& child2)
    {
        DockNode* child1Ptr = Utils::AllocateDockNode(std::move(child1));
        DockNode* child2Ptr = Utils::AllocateDockNode(std::move(child2));

        Stack(child1Ptr, child2Ptr);
    }

#if 0
    DockNode::DockNode(DockNode* parent, EDockSplitDir split, float32 splitPercent, DockNode* child1, DockNode* child2)
        : SplitDir(split), SplitPercent(splitPercent), Child1(child1), Child2(child2)
    {
        if (split != EDockSplitDir::Stack)
        {
            ImGuiDir dir = Utils::EDockSplitDirToImGuiDir(split);
            ImGuiID parentID;

            ImGuiID* child1ID = (ImGuiID*)child1->ID;
            ImGuiID* child2ID = (ImGuiID*)child2->ID;

            if (parent)
                parentID = parent->ID;
            else
            {
                const String& dockspaceName = ModuleSystem::Get<EditorModule>()->GetLayoutManager().GetDockspaceName();
				parentID = ImGui::GetID(dockspaceName.c_str());
            }

			ImGui::DockBuilderSplitNode(parentID, dir, splitPercent, child1ID, child2ID);
        }
        else
        {

        }

    }
#endif

	void Layout::ConstructDefaultLayout()
	{
		mName = "Default";

        const Ref<IPanel>& consolePanel        = ModuleSystem::Get<ConsolePanel>();    
        const Ref<IPanel>& viewportPanel       = ModuleSystem::Get<ViewportPanel>();
        const Ref<IPanel>& inspectorPanel      = ModuleSystem::Get<InspectorPanel>();
        const Ref<IPanel>& sceneHierarchyPanel = ModuleSystem::Get<SceneHierarchyPanel>();
        const Ref<IPanel>& contentBrowserPanel = ModuleSystem::Get<ContentBrowserPanel>();

        mRoot = DockNode::Split(
            EDockSplitDir::Left, 0.4f,
			DockNode::Split(
                EDockSplitDir::Up, 0.5f,
                DockNode(sceneHierarchyPanel),
                DockNode(inspectorPanel)
            ),
            DockNode::Split(
                EDockSplitDir::Up, 0.7f,
                DockNode(viewportPanel),
                DockNode::Stack(
                    DockNode(contentBrowserPanel),
                    DockNode(consolePanel)
                )
            )
		);
	}

	void Layout::ConstructFromCurrentLayout()
	{
		mName = "Custom";

	}

    void Layout::Load()
    {
        ImGuiID dockspaceID = ImGui::GetID(ModuleSystem::Get<DockspaceModule>()->GetDockspaceName().c_str());
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::DockBuilderRemoveNode(dockspaceID);
        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);

        mRoot->ID = (DockNodeID)dockspaceID;

        Utils::IterateDockNode(mRoot, [](DockNode* node)
        {
			if (!node->Child1)
				return;

			ImGuiDir splitDir = Utils::EDockSplitDirToImGuiDir(node->SplitDir);

            if (node->SplitDir != EDockSplitDir::Stack)
            {
                ImGuiID* child1ID = (ImGuiID*)node->Child1;
                ImGuiID* child2ID = (ImGuiID*)node->Child2;

                ImGui::DockBuilderSplitNode((ImGuiID)node->ID, splitDir, node->SplitPercent, child1ID, child2ID);
            }
            else
            {
                node->Child1->ID = node->ID;
            }

            if (node->Child1)
            {
				if (Ref<IPanel> panel = node->Child1->Panel.Own())
                    ImGui::DockBuilderDockWindow(panel->GetPanelName().c_str(), node->Child1->ID);
            }

            if (node->Child2)
            {
				if (Ref<IPanel> panel = node->Child2->Panel.Own())
                    ImGui::DockBuilderDockWindow(panel->GetPanelName().c_str(), node->Child2->ID);
            }
		});

        ImGui::DockBuilderFinish(dockspaceID);
    }

}
