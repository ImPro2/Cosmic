#include "cspch.hpp"
#include "MenubarModule.hpp"

#include "App/Input.hpp"
#include "App/PersistentStackAllocator.hpp"

#include "UI/ImGuiUtil.hpp"
#include "imgui.h"

namespace Cosmic
{

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

    void MenubarModule::OnEvent(const IEvent& e)
    {
        EventDispatcher dispatcher(e);

        CS_DISPATCH_EVENT(KeyPressEvent, OnKeyPressed);
    }

    void MenubarModule::RenderMenu(MenubarMenu* menu)
    {
        if (!ImGuiUtil::BeginMenu(menu->Name, menu->Keys, menu == mLayout.GetCurrentMenu()))
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

    void MenubarModule::ExecuteKeyPress(MenubarEntry* entry)
    {
		bool execute = entry->Keys.empty() ? false : true;

		for (EKeyCode key : entry->Keys)
			execute = execute && Input::IsKeyPressed(key);

		if (entry->IsMenu())
		{
			MenubarMenu* menu = (MenubarMenu*)entry;

			for (MenubarEntry* child : menu->Children)
				ExecuteKeyPress(child);

			if (execute)
				mLayout.SetCurrentMenu(menu);
		}
		else if (execute)
		{
			MenubarItem* item = (MenubarItem*)entry;
			item->Callback();
		}
    }

    bool MenubarModule::OnKeyPressed(const KeyPressEvent& e)
    {
        for (MenubarMenu* menu : mLayout.GetMenubar())
            ExecuteKeyPress(menu);

        return false;
    }

}
