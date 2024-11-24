#pragma once
#include "App/KeyAndMouseCodes.hpp"
#include "App/Module.hpp"
#include "App/KeyAndMouseCodes.hpp"

#include <imgui.h>
#include <initializer_list>

namespace Cosmic
{

    struct MenubarMenu;

    struct MenubarEntry
    {
        MenubarEntry(const char* name, const char* shortcut, std::initializer_list<EKeyCode> keys)
            : Name(name), Shortcut(shortcut), Keys(keys)
        {
        }

        virtual bool IsMenu() = 0;

        const char*      Name;
        const char*      Shortcut;
        Vector<EKeyCode> Keys;

        MenubarMenu* Parent;
    };

    struct MenubarItem : public MenubarEntry
    {
        MenubarItem(const char* name, const char* shortcut, const std::initializer_list<EKeyCode> keys, bool* enabledPtr = nullptr, std::function<void()> callback = {})
            : MenubarEntry(name, shortcut, keys), EnabledPtr(enabledPtr), Callback(callback)
        {
        }

        bool IsMenu() override { return false; }
        
        bool* EnabledPtr;
        std::function<void()> Callback;
    };

    struct MenubarMenu : public MenubarEntry
    {
        MenubarMenu(const char* name, const char* shortcut, const std::initializer_list<EKeyCode> keys)
            : MenubarEntry(name, shortcut, keys)
        {
        }

        bool IsMenu() override { return true; }

        Vector<MenubarEntry*> Children;
    };

    struct MenubarLayout
    {
        MenubarLayout();

        void BeginMenu(MenubarMenu* menu);
        void EndMenu();
        void Item(MenubarItem* item);

        Vector<MenubarMenu*> GetMenubar() const { return mMenubar; }
        
    private:
        Vector<MenubarMenu*> mMenubar;
        MenubarMenu* mCurrentMenu;
    };

    class MenubarModule : public IModule
    {
    public:
        MenubarModule(const MenubarLayout& layout);
        virtual ~MenubarModule();

    public:
        void OnImGuiRender() override;

    private:
        void RenderMenu(MenubarMenu* menu);

    private:
        void DeleteMenu(MenubarMenu* menu);

    private:
        MenubarLayout mLayout;
    };

}
