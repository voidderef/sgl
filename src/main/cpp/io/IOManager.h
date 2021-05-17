#pragma once

#include "ks/DynamicLibrary.h"
#include "ks/Object.h"
#include "ks/Settings.h"
#include "ks/ThreadLoop.h"
#include "ks/Timer.h"

#include "InputStateProcessor.h"
#include "IODevice.h"
#include "IOException.h"
#include "IOManagerCtx.h"
#include "IOStateProcessorManager.h"
#include "OutputStateProcessor.h"

namespace io {

class IOManager : public ks::Object, protected ks::ThreadLoop
{
public:
    IOManager(IOManagerCtx* ioManagerCtx, IOStateProcessorManager* stateProcessorManager);

    ~IOManager() override;

    bool IsInitialized() const {
        return m_initialized.load(std::memory_order_relaxed);
    }

    bool Init(ks::Settings& settings);

    void Shutdown();

    void SetIOProcessors(const std::string& key);

    void StateProcess();

    void FlushOutputStates();

    const InputState& GetInputStates() const {
        return m_inputState;
    }

    OutputState& GetOutputStates() {
        return m_outputState;
    }

protected:
    void _BeforeRunLoop() override;

    void _RunLoop() override;

private:
    struct DeviceState {
        IODevice* m_device {};
        InputState* m_inputState {};
        InputState* m_prevInputState {};
        OutputState* m_outputState {};
        bool m_outputSentToDevice;
        bool m_inputRecvFromDevice;
        InputStateProcessor* m_refInputStateProcessor {};
        OutputStateProcessor* m_refOutputStateProcessor {};

        explicit DeviceState(IODevice* device) :
            m_device(device),
            m_inputState(new InputState()),
            m_prevInputState(new InputState()),
            m_outputState(new OutputState()),
            m_refInputStateProcessor(nullptr),
            m_refOutputStateProcessor(nullptr)
        {};

        ~DeviceState() {
            delete m_device;
            delete m_inputState;
            delete m_prevInputState;
            delete m_outputState;
        }
    };

private:
    std::atomic<bool> m_initialized;

    IOManagerCtx* m_ioManagerCtx;
    IOStateProcessorManager* m_stateProcessorManager;

    uint16_t m_updateRateHz;

    ks::Timer m_updateTimer;
    ks::Timer m_ioProcessorSwapTimer;

    ks::Timer m_deviceUpdateTimer;

    std::vector<ks::DynamicLibrary*> m_ioLibs;
    std::vector<DeviceState*> m_deviceStates;

    std::atomic<bool> m_stateSwapProcessed;
    InputState m_inputState;
    OutputState m_outputState;

    std::atomic<uint32_t> m_swapLock;

private:
    void __LoadIOLibs(const std::string& libPath, const std::vector<std::string>& libsToLoad, ks::Settings& settings);

    inline void __LockStateSwapLock() {
        uint32_t expected = m_swapLock.load(std::memory_order_relaxed);

        // reserve swapping, tell io thread we want to swap
        while (!m_swapLock.compare_exchange_strong(expected, expected | 0x01, std::memory_order_acquire)) {
            _Yield();
        }

        // if the io thread is still in the swap section, we wait for the swap to complete
        // it won't enter this section again because we already reserved it above
        expected = 0x01;
        while (!m_swapLock.compare_exchange_strong(expected, 0x03, std::memory_order_acquire)) {
            expected = 0x01;
            _Yield();
        }
    }

    inline void __LockStateSwapUnlock() {
        uint32_t expected = 0x03;
        while (!m_swapLock.compare_exchange_strong(expected, 0x00, std::memory_order_acquire)) {
            throw IOException("Invalid concurrency state on state swap");
        }
    }

    inline void __LockStateSwapIoThreadLock() {
        uint32_t expected = 0;

        // if the main thread has reserved a swap already, wait (unfair for io thread)
        while (!m_swapLock.compare_exchange_strong(expected, 0x02, std::memory_order_acquire)) {
            expected = 0;
            _Yield();
        }
    }

    inline void __LockStateSwapIoThreadUnlock() {
        // if the main thread has already signaled that he wants to swap,
        // clear our own lock flag only
        uint32_t expected = m_swapLock.load(std::memory_order_relaxed);

        while (!m_swapLock.compare_exchange_strong(expected, expected & ~0x02, std::memory_order_acquire)) {
            _Yield();
        }
    }
};

}
