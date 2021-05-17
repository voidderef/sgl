#pragma once

#include <chrono>
#include <memory>
#include <thread>
#include <utility>

#include "ks/Exception.h"
#include "ks/Logger.hpp"

// Macro on windows
#ifdef Yield
#undef Yield
#endif

namespace ks {

class Thread
{
public:
    static void Yield()
    {
        std::this_thread::yield();
    }

    static void SleepMs(uint32_t timeMs)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeMs));
    }

    static void SleepUs(uint32_t timeUs)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(timeUs));
    }

public:
    /**
     * Constructor
     *
     * @param name Name of the thread (for debugging)
     */
    explicit Thread(std::string name = "") :
        m_name(std::move(name)),
        m_thread(nullptr)
    {}

    /**
     * Destructor
     */
    virtual ~Thread() = default;

    /**
     * Get the thread name
     */
    const std::string& GetName() const {
        return m_name;
    }

    /**
     * Start the thread. A thread can be restarted if it finished execution
     */
    virtual void Start()
    {
        m_thread = new std::thread(&Thread::__Run, this);
    }

    /**
     * Join the started thread
     */
    void Join()
    {
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
    }

protected:
    /**
     * Method executed by new thread. Implement this
     */
    virtual void _Run() = 0;

    /**
     * Yield this thread
     */
    void _Yield()
    {
        std::this_thread::yield();
    }

    /**
     * Put thread to sleep
     * @param timeMs Number of ms to sleep
     */
    void _SleepMs(uint32_t timeMs)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeMs));
    }

    void _SleepUs(uint32_t timeUs)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(timeUs));
    }

    void _SleepNs(uint32_t timeNs)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(timeNs));
    }

private:
    const std::string m_name;
    std::thread* m_thread;

    void __Run()
    {
        try {
            KS_LOG_INFO("Started thread %s", m_name);
            _Run();
            KS_LOG_INFO("Finished thread %s", m_name);
        } catch (Exception& e) {
            e.PrintStackTrace();
            throw e;
        }
     }
};

}
