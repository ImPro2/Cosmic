#include "cspch.hpp"
#include "Script/NativeScriptMacros.hpp"
#include "App/Application.hpp"

extern "C"
{

	CS_DLLEXPORT void CSInit(Cosmic::Application* instance)
	{
		Cosmic::Application::Set(instance);

	}

}
