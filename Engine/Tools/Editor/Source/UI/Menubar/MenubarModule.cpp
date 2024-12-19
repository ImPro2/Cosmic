#include "cspch.hpp"
#include "MenubarModule.hpp"

#include "App/Input.hpp"
#include "Memory/Memory.hpp"

#include "Editor/EditorModule.hpp"

#include "UI/ImGuiUtils/ImGuiUtils.hpp"
#include "imgui.h"

namespace Cosmic
{

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
        if (!ImGuiUtils::BeginMenu(menu->Name, menu->Keys, menu == mLayout.GetCurrentMenu()))
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
                
                if (ImGuiUtils::MenuItem(item->Name, item->Shortcut, item->Keys, item->EnabledPtr, item->Enabled) && item->Callback)
                    item->Callback();
            }

            if (entry->Separator)
                ImGui::Separator();
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
