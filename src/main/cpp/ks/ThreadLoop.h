#pragma once

#include <atomic>

#include "Thread.h"

namespace ks {

class ThreadLoop : public Thread
{
public:
    /**
     * Constructor
     *
     * @param name Name of the thread (for debugging)
     */
    explicit ThreadLoop(const std::string& name = "") :
        Thread(name),
        m_run(true),
        m_isRunning(false)
    {}

    /**
     * Destructor
     */
    virtual ~ThreadLoop() = default;

    bool IsRunning() const {
        return m_isRunning.load(std::memory_order_relaxed);
    }

    /**
     * Overriding
     */
    virtual void Start()
    {
        m_run = true;
        Thread::Start();
        m_isRunning.store(true, std::memory_order_release);
    }

    /**
     * Signal the thread to stop. This will cause the thread to exit
     * the loop before continuing with the next iteration.
     */
    virtual void Stop()
    {
        m_run = false;
        Join();
    }

protected:
    /**
     * Execute something before the thread starts looping (e.g. init/setup)
     */
    virtual void _BeforeRunLoop() {};

    /**
     * Run function which is looped until exit is signaled
     */
    virtual void _RunLoop() = 0;

    /**
     * Execute something after the loop exited and before the thread terminates
     * (e.g. cleanup)
     */
    virtual void _AfterRunLoop() {};

    void exitLoop() {
        m_run = false;
    }

    void _Run() override
    {
        _BeforeRunLoop();

        while (m_run) {
            _RunLoop();
        }

        _AfterRunLoop();
        m_isRunning.store(false, std::memory_order_release);
    }

private:
    std::atomic<bool> m_run;
    std::atomic<bool> m_isRunning;
};

}
