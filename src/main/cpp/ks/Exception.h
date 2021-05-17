#pragma once

#include <cstdarg>
#include <exception>
#include <string>

#include <backwards/backward.hpp>
#include <fmt/printf.h>
#include <fmt/ostream.h>

// Avoid collision with GetMessage macro of windows function
#ifdef KS_PLATFORM_WIN32
#undef GetMessage
#endif

namespace ks {

class Exception : public std::runtime_error
{
public:
    template<typename... Args>
    explicit Exception(const std::string& format, Args... args) :
        std::runtime_error(fmt::sprintf(format, args...))
    {
        LoadStackTrace();
    }

    /**
     * Constructor
     *
     * @param msg Exception message (c-string)
     */
    explicit Exception(const char* msg) :
        std::runtime_error(msg)
    {
        LoadStackTrace();
    }

    void LoadStackTrace() {
        m_stackTrace.load_here(32);
    }

    /**
     * Print the stack trace where the exception was created
     */
    void PrintStackTrace() {
        backward::Printer p;
        p.print(m_stackTrace);
    }

private:
    backward::StackTrace m_stackTrace;
};

}
