#pragma once

#include <string>
#include <format>

#define CS_LOG_TRACE(msg, ...)                                                                                                                                               \
do                                                                                                                                                                               \
{                                                                                                                                                                                \
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));               \
    ::Cosmic::Log::Trace(finalMsg.c_str(), __VA_ARGS__);                                                                                                                         \
} while (0)

#define CS_LOG_DEBUG(msg, ...)                                                                                                                                               \
do                                                                                                                                                                               \
{                                                                                                                                                                                \
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));               \
    ::Cosmic::Log::Debug(finalMsg.c_str(), __VA_ARGS__);                                                                                                                         \
} while (0)

#define CS_LOG_INFO(msg, ...)                                                                                                                                                \
do                                                                                                                                                                               \
{                                                                                                                                                                                \
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));               \
    ::Cosmic::Log::Info(finalMsg.c_str(), __VA_ARGS__);                                                                                                                          \
} while (0)

#define CS_LOG_WARN(msg, ...)                                                                                                                                                \
do                                                                                                                                                                               \
{                                                                                                                                                                                \
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));               \
    ::Cosmic::Log::Warn(finalMsg.c_str(), __VA_ARGS__);                                                                                                                          \
} while (0)

#define CS_LOG_ERROR(msg, ...)                                                                                                                                               \
do                                                                                                                                                                               \
{                                                                                                                                                                                \
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));               \
    ::Cosmic::Log::Error(finalMsg.c_str(), __VA_ARGS__);                                                                                                                         \
} while (0)

#define CS_LOG_CRITICAL(msg, ...)                                                                                                                                            \
do                                                                                                                                                                               \
{                                                                                                                                                                                \
    std::string finalMsg = std::vformat("[{}.{}]: {}", std::make_format_args(_CosmicModuleLogInfo::ProjectLogName(), _CosmicModuleLogInfo::ModuleLogName(), msg));               \
    ::Cosmic::Log::Critical(finalMsg.c_str(), __VA_ARGS__);                                                                                                                      \
} while (0)