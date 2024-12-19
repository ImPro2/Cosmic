#include "cspch.hpp"
#include "Script/NativeScriptMacros.hpp"
#include "Script/NativeScriptEngine.hpp"
#include "App/Application.hpp"

#include "Scripts/PlayerScript.hpp"

extern "C"
{

	CS_NATIVE_SCRIPT(PlayerScript);

	CS_DLLEXPORT void CSInit(Cosmic::Application* instance)
	{
		Cosmic::Application::Set(instance);

		CS_REGISTER_NATIVE_SCRIPT(PlayerScript);
	}

}
