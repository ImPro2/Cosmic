#include "cspch.hpp"
#include "MenubarLayout.hpp"

#include "Editor/EditorModule.hpp"

namespace Cosmic
{

    MenubarLayout MenubarLayout::Default()
    {
        Ref<EditorModule> editorModule = ModuleSystem::Get<EditorModule>();

        ActionManager& actionManager = editorModule->GetActionManager();
        Panels&        panels        = editorModule->GetPanels();
        LayoutManager& layoutManager = editorModule->GetLayoutManager();

        MenubarLayout menubar;

        menubar.BeginMenu(MenubarMenu("File", "ALT+F", { EKeyCode::LeftAlt, EKeyCode::F }));

        menubar.Item(MenubarItem("Open Scene",    "CTRL+O",       { EKeyCode::LeftControl, EKeyCode::O },                      nullptr, [&]() { editorModule->OpenScene();   }));
        menubar.Item(MenubarItem("Save Scene",    "CTRL+S",       { EKeyCode::LeftControl, EKeyCode::S },                      nullptr, [&]() { editorModule->SaveScene();   }));
        menubar.Item(MenubarItem("Save Scene As", "CTRL+SHIFT+S", { EKeyCode::LeftControl, EKeyCode::LeftShift, EKeyCode::S }, nullptr, [&]() { editorModule->SaveSceneAs(); }));

        menubar.EndMenu();

        menubar.BeginMenu(MenubarMenu("Edit", "ALT+E", { EKeyCode::LeftAlt, EKeyCode::E }));

        menubar.Item(MenubarItem("Undo", "CTRL+Z", { EKeyCode::LeftControl, EKeyCode::Z }, nullptr, [&]() { actionManager.UndoLastAction(); }));
        menubar.Item(MenubarItem("Redo", "CTRL+Y", { EKeyCode::LeftControl, EKeyCode::Y }, nullptr, [&]() { actionManager.RedoLastAction(); }));

        menubar.EndMenu();

        menubar.BeginMenu(MenubarMenu("View", "ALT+V", { EKeyCode::LeftAlt, EKeyCode::V }));
        menubar.BeginMenu(MenubarMenu("Panels", "", {}));

        menubar.Item(MenubarItem("Show All", "", {}, nullptr, [&]() { panels.ShowAll(); }));

        for (Ref<IPanel> panel : panels.GetPanels())
            menubar.Item(MenubarItem(panel->GetPanelName().c_str(), "", {}, panel->IsOpenPtr()));

        menubar.EndMenu();

        menubar.BeginMenu(MenubarMenu("Layouts", "", { }));

        menubar.Item(MenubarItem("Custom Layout...", "", {}, nullptr, [&]() { layoutManager.SaveCurrentLayout(); }));
        menubar.Separator();

        for (Layout& layout : layoutManager.GetLayouts())
        {
            bool loaded = layout.IsLoaded();
            menubar.Item(MenubarItem(layout.GetName().c_str(), "", {}, &loaded, [&]() { layoutManager.SwitchLayout(layout); }));
        }

        menubar.EndMenu();

        menubar.Item(MenubarItem("Show ImGui Demo Window", "", {}, editorModule->GetShowImGuiDemoWindowPtr()));

        menubar.SetCurrentMenu(nullptr);

        return menubar;
    }

    void MenubarLayout::BeginMenu(MenubarMenu&& menu)
    {
        MenubarMenu* ptr = PersistentStackAllocator::Allocate<MenubarMenu>(std::move(menu));

        if (mCurrentMenu == nullptr)
        {
            ptr->Parent = nullptr;
            mMenubar.emplace_back(ptr);
            mCurrentMenu = ptr;
        }
        else
        {
            ptr->Parent = mCurrentMenu;
            mCurrentMenu->Children.push_back(ptr);
            mCurrentMenu = ptr;
        }
    }

    void MenubarLayout::EndMenu()
    {
        mCurrentMenu = (MenubarMenu*)mCurrentMenu->Parent;
    }

    void MenubarLayout::Item(MenubarItem&& item)
    {
        MenubarItem* ptr = PersistentStackAllocator::Allocate<MenubarItem>(std::move(item));
        mCurrentMenu->Children.push_back(ptr);
    }

    void MenubarLayout::Separator()
    {
        mCurrentMenu->Children[mCurrentMenu->Children.size() - 1]->Separator = true;
    }

}
