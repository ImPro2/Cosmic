#pragma once
#include "Base/Base.hpp"
#include "Memory/SmartPtrs.hpp"

namespace Cosmic
{

	using DockNodeID = uint32;
	class IPanel;

	enum class EDockSplitDir
	{
		Left, Up, Right, Down,
		Stack
	};

	struct DockNode
	{
		DockNode()                = default;
		DockNode(const DockNode&) = default;
		DockNode(const WeakRef<IPanel>& panel);

		static DockNode* Split(EDockSplitDir dir, float32 splitPercent, DockNode*  child1, DockNode*  child2);
		static DockNode* Split(EDockSplitDir dir, float32 splitPercent, DockNode*  child1, DockNode&& child2);
		static DockNode* Split(EDockSplitDir dir, float32 splitPercent, DockNode&& child1, DockNode*  child2);
		static DockNode* Split(EDockSplitDir dir, float32 splitPercent, DockNode&& child1, DockNode&& child2);

		static DockNode* Stack(DockNode*  child1, DockNode*  child2);
		static DockNode* Stack(DockNode*  child1, DockNode&& child2);
		static DockNode* Stack(DockNode&& child1, DockNode*  child2);
		static DockNode* Stack(DockNode&& child1, DockNode&& child2);

		DockNodeID      ID;
		WeakRef<IPanel> Panel;

		EDockSplitDir   SplitDir;
		float32         SplitPercent;

		DockNode*       Child1;
		DockNode*       Child2;
	};

	class Layout
	{
	public:
		Layout()              = default;
		Layout(const Layout&) = default;

	public:
		void ConstructDefaultLayout();
		void ConstructFromCurrentLayout();

	public:
		void Load();

	private:
		String    mName;
		DockNode* mRoot;
	};

}
