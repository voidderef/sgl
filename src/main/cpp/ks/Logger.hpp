#pragma once

#include "Logger.h"

/**
 * Put this at the top of each file you call logging functions in and define
 * a name for the module. This name is added to the log strings on log calls
 */
#ifndef LOG_MODULE
#define S1(x) #x
#define S2(x) S1(x)
#define LOG_MODULE strrchr(__FILE__ ":" S2(__LINE__), '/') ? \
    strrchr(__FILE__ ":" S2(__LINE__), '/') + 1 : __FILE__ ":" S2(__LINE__)
#endif

/**
 * Macro to log a message, level panic. Use this macro instead of directly
 * calling the logger class.
 */
#define KS_LOG_PANIC(fmt, ...) ks::Logger::GetLogger()->critical(("[%s] " + std::string(fmt)).c_str(), LOG_MODULE, ##__VA_ARGS__)

/**
 * Macro to log a message, level error. Use this macro instead of directly
 * calling the logger class.
 */
#define KS_LOG_ERROR(fmt, ...) ks::Logger::GetLogger()->error(("[%s] " + std::string(fmt)).c_str(), LOG_MODULE, ##__VA_ARGS__)

/**
 * Macro to log a message, level warning. Use this macro instead of directly
 * calling the logger class.
 */
#define KS_LOG_WARN(fmt, ...) ks::Logger::GetLogger()->warn(("[%s] " + std::string(fmt)).c_str(), LOG_MODULE, ##__VA_ARGS__)

/**
 * Macro to log a message, level info. Use this macro instead of directly
 * calling the logger class.
 */
#define KS_LOG_INFO(fmt, ...) ks::Logger::GetLogger()->info(("[%s] " + std::string(fmt)).c_str(), LOG_MODULE, ##__VA_ARGS__)

/**
 * Macro to log a message, level debug. Use this macro instead of directly
 * calling the logger class.
 */
#define KS_LOG_DEBUG(fmt, ...) ks::Logger::GetLogger()->debug(("[%s] " + std::string(fmt)).c_str(), LOG_MODULE, ##__VA_ARGS__)


//#define KS_LOG_TRACE_STRIP
#ifndef KS_LOG_TRACE_STRIP

/**
 * Macro to log a message, level trace. Use this macro instead of directly
 * calling the logger class.
 */
#define KS_LOG_TRACE(fmt, ...) ks::Logger::GetLogger()->trace(("[%s] " + std::string(fmt)).c_str(), LOG_MODULE, ##__VA_ARGS__)

/**
 * Macro to easily trace function calls. Just add this at the top of a
 * function's body.
 */
#define KS_LOG_TRACE_FUNC IBNET_LOG_TRACE("%s %s", "ENTER", __PRETTY_FUNCTION__)

/**
 * Macro to easily trace function calls. Just add this at the bottom of a
 * function's body.
 */
#define KS_LOG_TRACE_FUNC_EXIT KS_LOG_TRACE("%s %s", "EXIT", __PRETTY_FUNCTION__)
#else
#define KS_LOG_TRACE(fmt, ...)
#define KS_LOG_TRACE_FUNC
#define KS_LOG_TRACE_FUNC_EXIT
#endif
