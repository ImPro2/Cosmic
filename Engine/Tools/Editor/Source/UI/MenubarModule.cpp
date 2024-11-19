#include "cspch.hpp"
#include "MenubarModule.hpp"

#include "UI/ImGuiUtil.hpp"
#include "imgui.h"

namespace Cosmic
{

    MenubarLayout::MenubarLayout()
    {
        mCurrentMenu = nullptr;
    }

    void MenubarLayout::BeginMenu(MenubarMenu* menu)
    {
        if (mCurrentMenu == nullptr)
        {
            menu->Parent = nullptr;
            mMenubar.emplace_back(menu);            
            mCurrentMenu = menu;
        }
        else
        {
            menu->Parent = mCurrentMenu;
            mCurrentMenu->Children.push_back(menu);
            mCurrentMenu = menu;
        }
    }

    void MenubarLayout::EndMenu()
    {
        mCurrentMenu = (MenubarMenu*)mCurrentMenu->Parent;
    }

    void MenubarLayout::Item(MenubarItem* item)
    {
        mCurrentMenu->Children.push_back(item);
    }

    MenubarModule::MenubarModule(const MenubarLayout& layout)
        : mLayout(layout)
    {
    }

    MenubarModule::~MenubarModule()
    {
        for (MenubarMenu* menu : mLayout.GetMenubar())
            DeleteMenu(menu);
    }

    void MenubarModule::OnImGuiRender()
    {
        if (ImGui::BeginMainMenuBar())
        {
            for (MenubarMenu* menu : mLayout.GetMenubar())
                RenderMenu(menu);

            ImGui::EndMainMenuBar();
        }
    }

    void MenubarModule::RenderMenu(MenubarMenu* menu)
    {
        if (!ImGuiUtil::BeginMenu(menu->Name, menu->Keys))
            return;

        for (MenubarEntry* entry : menu->Children)
        {
            if (entry->IsMenu())
            {
                MenubarMenu* menu = (MenubarMenu*)entry;
                RenderMenu(menu);
            }
            else
            {
                MenubarItem* item = (MenubarItem*)entry;
                
                if (ImGuiUtil::MenuItem(item->Name, item->Shortcut, item->Keys, item->EnabledPtr) && item->Callback)
                    item->Callback();
            }
        }

        ImGui::EndMenu();
    }

    void MenubarModule::DeleteMenu(MenubarMenu* menu)
    {
        for (MenubarEntry* entry : menu->Children)
        {
            if (entry->IsMenu())
                DeleteMenu((MenubarMenu*)entry);
            else
                delete (MenubarItem*)entry;
        }

        delete menu;
    }

}
