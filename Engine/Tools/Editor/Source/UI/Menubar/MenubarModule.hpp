#pragma once
#include "App/KeyAndMouseCodes.hpp"
#include "App/Module.hpp"
#include "App/KeyAndMouseCodes.hpp"

#include "UI/Menubar/MenubarLayout.hpp"

#include <imgui.h>
#include <initializer_list>

namespace Cosmic
{

    class MenubarModule : public IModule
    {
    public:
        MenubarModule(const MenubarLayout& layout);
        virtual ~MenubarModule();

    public:
        MenubarLayout& GetLayout()             { return mLayout; }
        const MenubarLayout& GetLayout() const { return mLayout; }

    public:
        void OnImGuiRender() override;
        void OnEvent(const IEvent& e) override;

    private:
        void RenderMenu(MenubarMenu* menu);
        void DeleteMenu(MenubarMenu* menu);

    private:
        void ExecuteKeyPress(MenubarEntry* entry);

    private:
        bool OnKeyPressed(const KeyPressEvent& e);

    private:
        MenubarLayout mLayout;
    };

}
