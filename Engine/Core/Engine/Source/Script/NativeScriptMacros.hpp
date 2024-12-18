#pragma once
#include "Base/Macros.hpp"

#ifdef CS_COMPILER_MSVC
#	define CS_NATIVE_SCRIPT(ScriptClass)                                                              \
	extern "C"                                                                                        \
	{                                                                                                 \
		CS_DLLEXPORT Cosmic::NativeScript* CSInstantiate##ScriptClass##(Cosmic::Entity entity)        \
		{                                                                                             \
			return Cosmic::NativeScriptEngine::AllocateNativeScript<##Cosmic::ScriptClass##>(entity); \
		}                                                                                             \
	}
#elif defined(CS_COMPILER_GCC)
#	define CS_NATIVE_SCRIPT(ScriptClass)                                                          \
	extern "C"                                                                                    \
	{                                                                                             \
		CS_DLLEXPORT Cosmic::NativeScript* CSInstantiateScriptClass(Cosmic::Entity entity)        \
		{                                                                                         \
			return Cosmic::NativeScriptEngine::AllocateNativeScript<Cosmic::ScriptClass>(entity); \
		}                                                                                         \
	}
#endif
