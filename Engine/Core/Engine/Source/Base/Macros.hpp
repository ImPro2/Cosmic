#pragma once
//#include <optick.h>

// Identify the compiler

#if defined(_MSC_VER)
#   define CS_COMPILER_MSVC
#elif defined(__clang__)
#   define CS_COMPILER_CLANG
#elif defined(__GNUC__)
#   define CS_COMPILER_GCC
#endif

// Identify target OS

#if defined(_WIN32)
#   if defined(_WIN64)
#       define CS_PLATFORM_WINDOWS
#   else
#       error Windows 32-bit is not supported.
#   endif

#elif defined(__APPLE__) || defined(__MACH__)
#   include <TargetConditionals.h>
#   if TARGET_IPHONE_SIMULATOR == 1
#       define CS_PLATFORM_IOS_SIMULATOR
#       error IOS Simulator is not supported.
#   elif TARGET_OS_IPHONE == 1
#       define CS_PLATFORM_IOS
#   elif TARGET_OS_MAC == 1
#       define CS_PLATFORM_MACOS
#   else
#       error Undefined Apple Platform.
#   endif

#elif defined(__ANDROID__)
#   define CS_PLATFORM_ANDROID
#   error Android is not supported.

#elif defined(__linux__)
#   define CS_PLATFORM_LINUX
//#   error Linux isn't suppported.

#else
#   error Undefined Platform.

#endif

// Debug/Assertions

#if defined(CS_COMPILER_MSVC)
#    define CS_BREAK()            __debugbreak()
#    define CS_FORCEINLINE        __forceinline
#    define CS_UNREACHABLE()      __assume(0)
#    define CS_TYPENAME_TO_STR(T) typeid(T).name()
#elif defined(CS_COMPILER_CLANG)
#    define CS_BREAK()            __builtin_debugtrap()
#    define CS_FORCEINLINE        __attribute__((always_inline))
#    define CS_UNREACHABLE()      __builtin_unreachable()
#elif defined(CS_COMPILER_GCC)
#    include <csignal>
#    define CS_BREAK()            std::raise(SIGTRAP)
#    define CS_FORCEINLINE        __attribute__((always_inline)) inline
#    define CS_UNREACHABLE()      __builtin_unreachable()
#endif

#ifdef CS_DEBUG
#   define CS_ASSERT(Condition, Message)                                                    \
    do                                                                                      \
    {                                                                                       \
        if (!(Condition))                                                                   \
        {                                                                                   \
            CS_LOG_ERROR("Assertion Failed [{}:{}:{}]:", __FILE__, __FUNCTION__, __LINE__); \
            CS_LOG_ERROR(Message);                                                          \
            CS_BREAK();                                                                     \
        }                                                                                   \
    } while(0)
#elif defined(CS_RELEASE) || defined(CS_DIST)
#   define CS_ASSERT(Condition, Message)
#endif

#define CS_INVALID_ENUM(type) CS_ASSERT(false, "Invalid enum of type `{}` reached.", #type)

#define CS_NOT_IMPLEMENTED() CS_ASSERT(false, "Feature not yet implemented.")

// Logging

#define CS_MODULE_LOG_INFO(prj, mod)                                \
namespace _CosmicModuleLogInfo                                      \
{                                                                   \
    static const std::string& ProjectLogName()                      \
    {                                                               \
        static const std::string projectLogName = #prj;             \
        return projectLogName;                                      \
    }                                                               \
                                                                    \
    static const std::string& ModuleLogName()                       \
    {                                                               \
        static const std::string moduleLogName = #mod;              \
        return moduleLogName;                                       \
    }                                                               \
}

// Events

#ifdef CS_COMPILER_MSVC
#   define CS_DISPATCH_EVENT(type, listener) dispatcher.Dispatch<##type##>([this](const type##& e) -> bool { return this->##listener##(e); })
#elif defined(CS_COMPILER_GCC)
#   define CS_DISPATCH_EVENT(type, listener) dispatcher.Dispatch<type>([this](const type& e) -> bool { return this->listener(e); })
#endif

// Profiling

//#define CS_PROFILE_RUN_LOOP(name) OPTICK_FRAME(name)
//#define CS_PROFILE_FN() OPTICK_EVENT()
#define CS_PROFILE_RUN_LOOP(name)
#define CS_PROFILE_FN()
