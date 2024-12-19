#pragma once
#include "Base/Base.hpp"
#include "Memory/Memory.hpp"

#include <functional>

namespace Cosmic
{

	using DockNodeID = uint32;
	class IPanel;

	enum class EDockSplitDir
	{
		None,
		Left, Up, Right, Down,
		Stack
	};

	const char*   EDockSplitDirToStr(EDockSplitDir dir);
	EDockSplitDir EDockSplitDirFromStr(const char* str);

	struct DockNode
	{
		template<typename... Args>
		static DockNode* Allocate(Args&&... args)
		{
            return PersistentStackAllocator::Allocate<DockNode>(std::forward<Args>(args)...);
		}

		DockNode()                = default;
		DockNode(const DockNode&) = default;
		DockNode(DockNode&&)      = default;
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

		EDockSplitDir   SplitDir     = EDockSplitDir::None;
		float32         SplitPercent = 0.0f;

		DockNode*       Parent       = nullptr;
		DockNode*       Child1       = nullptr;
		DockNode*       Child2       = nullptr;
	};

	class LayoutManager;

	using IterDockNodeCallback = std::function<bool(DockNode*)>;

	class Layout
	{
	public:
		Layout(const String& name = "")
			: mName(name)
		{
		}

	public:
		void IterDockNodes(IterDockNodeCallback callback);

	public:
		void ConstructDefaultLayout();
		void ConstructFromCurrentLayout();

	public:
		void Load();

	public:
		void SetRootNode(DockNode* root) { mRoot = root; }
		void SetName(const String& name) { mName = name; }

	public:
		DockNode*     GetRootNode()    { return mRoot;     }
		const String& GetName()  const { return mName;     }
		bool          IsLoaded() const { return mIsLoaded; }

	private:
		String    mName;
		DockNode* mRoot;
		bool      mIsLoaded = false;

		friend class LayoutManager;
	};

	inline bool operator==(const Layout& left, const Layout& right)
	{
		return (left.GetName() == right.GetName());
	}

}
