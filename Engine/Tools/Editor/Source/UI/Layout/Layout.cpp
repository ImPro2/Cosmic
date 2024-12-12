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

        Ref<IPanel> GetPanelFromWindowName(const String& name)
        {
            for (Ref<IPanel> panel : ModuleSystem::Get<EditorModule>()->GetPanels().GetPanels())
            {
                if (panel->GetPanelName() == name)
                    return panel;
            }
        }

        template<typename Callback>
        static DockNode* IterateImGuiDockNode(ImGuiDockNode* node, DockNode* currNode, Callback fn, DockNode* parent = nullptr)
        {
			currNode = AllocateDockNode();
            currNode->Parent = parent;

            fn(node, currNode);

            if (node->ChildNodes[0])
                currNode->Child1 = IterateImGuiDockNode(node->ChildNodes[0], currNode->Child1, fn, currNode);
            if (node->ChildNodes[1])
                currNode->Child2 = IterateImGuiDockNode(node->ChildNodes[1], currNode->Child2, fn, currNode);

            return currNode;
        }

        // Breadth-first
        template<typename Callback>
        static bool IterateDockNode(DockNode* node, Callback fn)
        {
            bool exit = fn(node);

            if (exit)
                return true;

            if (node->Child1)
                exit = IterateDockNode(node->Child1, fn);

            if (exit)
                return true;

            if (node->Child2)
                exit = IterateDockNode(node->Child2, fn);

            return exit;
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

        return Split(dir, splitPercent, child1, child2Ptr);
    }

    DockNode* DockNode::Split(EDockSplitDir dir, float32 splitPercent, DockNode&& child1, DockNode* child2)
    {
        DockNode* child1Ptr = Utils::AllocateDockNode(std::move(child1));

        return Split(dir, splitPercent, child1Ptr, child2);
    }

    DockNode* DockNode::Split(EDockSplitDir dir, float32 splitPercent, DockNode&& child1, DockNode&& child2)
    {
        DockNode* child1Ptr = Utils::AllocateDockNode(std::move(child1));
        DockNode* child2Ptr = Utils::AllocateDockNode(std::move(child2));

        return Split(dir, splitPercent, child1Ptr, child2Ptr);
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

        return Stack(child1, child2Ptr);
    }

    DockNode* DockNode::Stack(DockNode&& child1, DockNode* child2)
    {
        DockNode* child1Ptr = Utils::AllocateDockNode(std::move(child1));

        return Stack(child1Ptr, child2);
    }

    DockNode* DockNode::Stack(DockNode&& child1, DockNode&& child2)
    {
        DockNode* child1Ptr = Utils::AllocateDockNode(std::move(child1));
        DockNode* child2Ptr = Utils::AllocateDockNode(std::move(child2));

        return Stack(child1Ptr, child2Ptr);
    }

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
        ImGuiID dockspaceID = ImGui::GetID(ModuleSystem::Get<DockspaceModule>()->GetDockspaceName().c_str());

        ImGuiContext*  ctx  = ImGui::GetCurrentContext();
        ImGuiDockNode* root = (ImGuiDockNode*)ctx->DockContext.Nodes.GetVoidPtr(dockspaceID);

        Vector<Ref<IPanel>> panels = ModuleSystem::Get<EditorModule>()->GetPanels().GetOpenPanels();
        
		mRoot = Utils::IterateImGuiDockNode(root, nullptr, [&](ImGuiDockNode* node, DockNode* currNode)
		{
			currNode->ID = node->ID;

			if (node->IsLeafNode())
			{
                Ref<IPanel> panel = Utils::GetPanelFromWindowName(node->VisibleWindow->Name);
                currNode->Panel = panel;

                panels.erase(std::find(panels.begin(), panels.end(), panel));
			}
            else
            {
                if (node->IsSplitNode())
                {
                    ImVec2 child1Size = node->ChildNodes[0]->SizeRef;
                    ImVec2 child2Size = node->ChildNodes[1]->SizeRef;

                    switch (node->SplitAxis)
                    {
						case ImGuiAxis_None:
						{
							currNode->SplitDir = EDockSplitDir::Stack;
							break;
						}
						case ImGuiAxis_X:
						{
							currNode->SplitDir     = EDockSplitDir::Left;
                            currNode->SplitPercent = child1Size.x / (child1Size.x + child2Size.x);
							break;
						}
						case ImGuiAxis_Y:
                        {
                            currNode->SplitDir = EDockSplitDir::Up;
                            currNode->SplitPercent = child1Size.y / (child1Size.y + child2Size.y);
                            break;
                        }
                    }
                }
            }
		});

        for (Ref<IPanel> panel : panels)
        {
            ImGuiWindow* window = ImGui::FindWindowByName(panel->GetPanelName().c_str());

            Utils::IterateDockNode(mRoot, [&](DockNode* node) -> bool
			{
				if (window->DockId == node->ID)
				{
                    node->Parent->Child2 = DockNode::Stack(node->Parent->Child2, DockNode(panel));
                    return true;
                }

                return false;
			});
        }
	}

    void Layout::Load()
    {
        ImGuiID dockspaceID = ImGui::GetID(ModuleSystem::Get<DockspaceModule>()->GetDockspaceName().c_str());
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::DockBuilderRemoveNode(dockspaceID);
        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);

        mRoot->ID = (DockNodeID)dockspaceID;

        Utils::IterateDockNode(mRoot, [](DockNode* node) -> bool
        {
            switch (node->SplitDir)
            {
				case EDockSplitDir::None:
				{
					if (Ref<IPanel> panel = node->Panel.Own())
						ImGui::DockBuilderDockWindow(panel->GetPanelName().c_str(), node->ID);

					break;
				}
				case EDockSplitDir::Left:
				case EDockSplitDir::Right:
				case EDockSplitDir::Up:
				case EDockSplitDir::Down:
				{
					ImGuiDir splitDir = Utils::EDockSplitDirToImGuiDir(node->SplitDir);

                    ImGuiID child1ID;
                    ImGuiID child2ID;

					ImGui::DockBuilderSplitNode((ImGuiID)node->ID, splitDir, node->SplitPercent, &child1ID, &child2ID);

                    node->Child1->ID = child1ID;
                    node->Child2->ID = child2ID;

					break;
				}
				case EDockSplitDir::Stack:
				{
					node->Child1->ID = node->ID;
                    node->Child2->ID = node->ID;

					break;
				}
            }

			return false;
		});

        ImGui::DockBuilderFinish(dockspaceID);
    }

}
