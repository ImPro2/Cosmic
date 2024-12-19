#pragma once
#include "Base/Macros.hpp"

#ifdef CS_COMPILER_MSVC
#	define CS_NATIVE_SCRIPT_INSTANTIATE_FUNCTION_NAME(x) CSInstantiate##x
#	define CS_NATIVE_SCRIPT(ScriptClass)                                                                                 \
	CS_DLLEXPORT Cosmic::NativeScript* CS_NATIVE_SCRIPT_INSTANTIATE_FUNCTION_NAME(ScriptClass) (Cosmic::Entity entity)   \
	{                                                                                                                    \
		return Cosmic::NativeScriptEngine::AllocateNativeScript<##Cosmic::ScriptClass##>(entity);                        \
	}

#	define CS_REGISTER_NATIVE_SCRIPT(ScriptClass)                                                                         \
	Cosmic::NativeScriptEngine::GetRegistry().RegisterScriptClass(CS_STRINGIFY(ScriptClass), CS_NATIVE_SCRIPT_INSTANTIATE_FUNCTION_NAME(ScriptClass) )

#elif defined(CS_COMPILER_GCC)
#	define CS_NATIVE_SCRIPT(ScriptClass)                                                          \
		CS_DLLEXPORT Cosmic::NativeScript* CSInstantiateScriptClass(Cosmic::Entity entity)        \
		{                                                                                         \
			return Cosmic::NativeScriptEngine::AllocateNativeScript<Cosmic::ScriptClass>(entity); \
		}
#endif
