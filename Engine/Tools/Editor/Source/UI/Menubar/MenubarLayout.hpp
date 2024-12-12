#pragma once
#include "Base/Base.hpp"
#include "App/KeyAndMouseCodes.hpp"

#include <functional>

namespace Cosmic
{

	struct MenubarMenu;

    struct MenubarEntry
    {
        MenubarEntry(const char* name, const char* shortcut, const Vector<EKeyCode>& keys, MenubarMenu* parent = nullptr)
            : Name(name), Shortcut(shortcut), Keys(keys), Parent(Parent)
        {
        }

        virtual bool IsMenu() = 0;

        const char*      Name;
        const char*      Shortcut;
        Vector<EKeyCode> Keys;

        MenubarMenu* Parent;

        // Adds separator after
        bool Separator = false;
    };

    struct MenubarItem : public MenubarEntry
    {
        MenubarItem(const char* name, const char* shortcut, const std::initializer_list<EKeyCode> keys, bool* enabledPtr = nullptr, std::function<void()> callback = {}, bool enabled = false)
            : MenubarEntry(name, shortcut, keys), EnabledPtr(enabledPtr), Callback(callback), Enabled(Enabled)
        {
        }

        MenubarItem(const MenubarItem& other)
            : MenubarEntry(other.Name, other.Shortcut, other.Keys, other.Parent), EnabledPtr(other.EnabledPtr), Callback(other.Callback), Enabled(Enabled)
        {
        }

        bool IsMenu() override { return false; }
        
        bool  Enabled;
        bool* EnabledPtr;
        std::function<void()> Callback;
    };

    struct MenubarMenu : public MenubarEntry
    {
        MenubarMenu(const char* name, const char* shortcut, const std::initializer_list<EKeyCode> keys)
            : MenubarEntry(name, shortcut, keys)
        {
        }

        MenubarMenu(const MenubarMenu& other)
            : MenubarEntry(other.Name, other.Shortcut, other.Keys, other.Parent), Children(other.Children)
        {
        }

        bool IsMenu() override { return true; }

        Vector<MenubarEntry*> Children;
    };

    class MenubarLayout
    {
    public:
        static MenubarLayout Default();

        MenubarLayout()                     = default;
        MenubarLayout(const MenubarLayout&) = default;

        void BeginMenu(MenubarMenu&& menu);
        void EndMenu();
        void Item(MenubarItem&& item);
        void Separator();

        Vector<MenubarMenu*> GetMenubar() const { return mMenubar; }

        void         SetCurrentMenu(MenubarMenu* menu) { mCurrentMenu = menu; }
        MenubarMenu* GetCurrentMenu()                  { return mCurrentMenu; }

        MenubarItem* GetItem(const String& name);
        MenubarMenu* GetMenu(const String& name);
        
    private:
        Vector<MenubarMenu*> mMenubar;
        MenubarMenu* mCurrentMenu = nullptr;
    };

}
