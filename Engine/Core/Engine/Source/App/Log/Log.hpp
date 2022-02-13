#pragma once

#include <string>
#include <format>

#define CS_LOG_TRACE(msg, ...)\
do\
{\
    const char* console = "Default";\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Trace(finalMsg.c_str(), __VA_ARGS__, console);\
} while (0)

#define CS_LOG_DEBUG(msg, ...)\
do\
{\
    const char* console = "Default";\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Debug(finalMsg.c_str(), __VA_ARGS__, console);\
} while (0)

#define CS_LOG_INFO(msg, ...)\
do\
{\
    const char* console = "Default";\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Info(finalMsg.c_str(), __VA_ARGS__, console);\
} while (0)

#define CS_LOG_WARN(msg, ...)\
do\
{\
    const char* console = "Default";\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Warn(finalMsg.c_str(), __VA_ARGS__, console);\
} while (0)

#define CS_LOG_ERROR(msg, ...)\
do\
{\
    const char* console = "Default";\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Error(finalMsg.c_str(), __VA_ARGS__, console);\
} while (0)

#define CS_LOG_CRITICAL(msg, ...)\
do\
{\
    const char* console = "Default";\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Critical(finalMsg.c_str(), __VA_ARGS__, console);\
} while (0)

// specify which console to log to (also logs to file)

#define CS_LOG_TRACE_CONSOLE(console, msg, ...)\
do\
{\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Trace(finalMsg.c_str(), console, __VA_ARGS__);\
} while (0)

#define CS_LOG_DEBUG_CONSOLE(console, msg, ...)\
do\
{\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Debug(finalMsg.c_str(), console, __VA_ARGS__);\
} while (0)

#define CS_LOG_INFO_CONSOLE(console, msg, ...)\
do\
{\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Info(finalMsg.c_str(), console, __VA_ARGS__);\
} while (0)

#define CS_LOG_WARN_CONSOLE(console, msg, ...)\
do\
{\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Warn(finalMsg.c_str(), console, __VA_ARGS__);\
} while (0)

#define CS_LOG_ERROR_CONSOLE(console, msg, ...)\
do\
{\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Error(finalMsg.c_str(), console, __VA_ARGS__);\
} while (0)

#define CS_LOG_CRITICAL_CONSOLE(console, msg, ...)\
do\
{\
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));\
    ::Cosmic::Log::Critical(finalMsg.c_str(), console, __VA_ARGS__);\
} while (0)





