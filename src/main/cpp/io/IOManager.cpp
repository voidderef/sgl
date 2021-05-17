#include "IOManager.h"

#include "ks/FileSystem.h"

#include "LibIODevice.h"

namespace io {

IOManager::IOManager(IOManagerCtx* ioManagerCtx, IOStateProcessorManager* stateProcessorManager) :
    ThreadLoop("IOManager"),
    m_initialized(false),
    m_ioManagerCtx(ioManagerCtx),
    m_stateProcessorManager(stateProcessorManager),
    m_updateRateHz(60),
    m_stateSwapProcessed(true),
    m_swapLock(0)
{

}

IOManager::~IOManager()
{
    delete m_ioManagerCtx;
    delete m_stateProcessorManager;
}

bool IOManager::Init(ks::Settings& settings)
{
    KS_LOG_INFO("Initialize");

    __LoadIOLibs(settings.GetValue<const std::string&>("io" + ks::Settings::ms_delim + "path_modules"),
        settings.GetValue<const std::vector<std::string>&>("io" + ks::Settings::ms_delim + "modules"), settings);

    m_updateRateHz = settings.GetValue<uint16_t>("io" + ks::Settings::ms_delim + "update_rate_hz");

    m_stateSwapProcessed = true;

    Start();

    m_initialized.store(true, std::memory_order_release);

    return true;
}

void IOManager::Shutdown()
{
    FlushOutputStates();

    m_initialized.store(false, std::memory_order_acquire);

    Stop();

    for (auto& it : m_deviceStates) {
        it->m_device->Close();
        delete it;
    }

    for (auto it : m_ioLibs) {
        delete it;
    }
}

void IOManager::SetIOProcessors(const std::string& key)
{
    __LockStateSwapLock();

    KS_LOG_DEBUG("Switching to new io processor: %s", key);

    m_ioProcessorSwapTimer.Start();

    auto processors = m_stateProcessorManager->GetProcessors(key);

    // exchange processors on all devices
    for (auto& it : m_deviceStates) {
        auto it2 = processors.find(it->m_device->GetIdentifier());
        if (it2 != processors.end()) {
            KS_LOG_DEBUG("New io processors for device: %s", it->m_device->GetIdentifier());

            // deactivate old processors
            if (it->m_refInputStateProcessor) {
                it->m_refInputStateProcessor->Deactivate();
            }

            if (it->m_refOutputStateProcessor) {
                it->m_refOutputStateProcessor->Deactivate();
            }

            it->m_refInputStateProcessor = it2->second->m_input;
            it->m_refOutputStateProcessor = it2->second->m_output;

            // activate new processors

            if (it->m_refInputStateProcessor) {
                it->m_refInputStateProcessor->Activate();
            }

            if (it->m_refOutputStateProcessor) {
                it->m_refOutputStateProcessor->Activate();
            }
        } else {
            it->m_refInputStateProcessor = nullptr;
            it->m_refOutputStateProcessor = nullptr;
        }

        // don't clear current/prev input states and output states of device to avoid
        // bouncy in/outputs when switching io processors
        // it->m_inputState->Clear();
        // it->m_prevInputState->Clear();
        // it->m_outputState->Clear();
        // it->m_outputSentToDevice = true;
    }

    // null all buffer states
    m_inputState.Clear();

    m_outputState.Clear();

    m_ioProcessorSwapTimer.Stop();

    KS_LOG_DEBUG("IO processor swap time %f us", m_ioProcessorSwapTimer.GetUs());

    __LockStateSwapUnlock();
}

void IOManager::StateProcess()
{
    __LockStateSwapLock();

    // apply output state to all devices
    for (auto &it : m_deviceStates) {
        if (it->m_refOutputStateProcessor) {
            it->m_refOutputStateProcessor->Process(m_outputState, *it->m_outputState);
            it->m_outputSentToDevice = false;
        }
    }

    // done processing, clear
    m_outputState.Clear();

    // wipe previously processed state
    m_inputState.Clear();

    InputState tmpInputState;

    // evaluate input states
    for (auto& it : m_deviceStates) {
        if (it->m_inputRecvFromDevice) {
            tmpInputState.Clear();

            // eval current state and previous
            if (it->m_refInputStateProcessor) {
                // TODO this part here lacks merging input states of multiple devices properly
                // currently, we are using the same state object which works for digital inputs
                // but doesn't for analog (destination input state)
                it->m_refInputStateProcessor->Process(*it->m_inputState, *it->m_prevInputState, tmpInputState,
                    m_outputState);
            }

            // state swap after eval to store previous state for next swap and evaluation
            InputState* tmp = it->m_prevInputState;
            it->m_prevInputState = it->m_inputState;
            it->m_inputState = tmp;

            // merge input states
            m_inputState.Merge(tmpInputState);
            it->m_inputRecvFromDevice = false;
        }
    }

    m_stateSwapProcessed.store(false, std::memory_order_release);

    __LockStateSwapUnlock();
}

void IOManager::FlushOutputStates()
{
    while (!m_stateSwapProcessed.load(std::memory_order_relaxed)) {
        ks::Thread::SleepMs(1);
    }
}

void IOManager::_BeforeRunLoop()
{
    m_updateTimer.Start();
}

void IOManager::_RunLoop()
{
    m_deviceUpdateTimer.Start();

    __LockStateSwapIoThreadLock();

    for (auto& it : m_deviceStates) {
        // apply output state once to avoid pumping cleared and probably invalid
        // output states to the hardware
        if (!it->m_outputSentToDevice) {
            it->m_device->ProcessOutputStates(*it->m_outputState);
            it->m_outputState->Clear();
            it->m_outputSentToDevice = true;
        }

        try {
            it->m_device->Update();
        } catch (ks::Exception& e) {
            KS_LOG_ERROR("Updating device '%s' failed", it->m_device->GetIdentifier());
            // TODO remove device from runtime? -> hotplugging?
            continue;
        }

        // poll input states to keep the current one updated
        if (!it->m_inputRecvFromDevice) {
            it->m_inputState->Clear();
            it->m_device->ProcessInputStates(*it->m_inputState);
            it->m_inputRecvFromDevice = true;
        }
    }

    m_stateSwapProcessed.store(true, std::memory_order_release);

    __LockStateSwapIoThreadUnlock();

    m_deviceUpdateTimer.Stop();

    // control loop by update rate
    double deltaNs = m_updateTimer.GetNs();
    double frameTime = (1000.0 * 1000.0 * 1000.0) / m_updateRateHz;

    if (deltaNs <= frameTime) {
        _SleepNs((uint32_t) (frameTime - deltaNs));
    } else {
        // TODO frameskipping/catchup?
        KS_LOG_WARN("IO thread frame took too long: %f ms\nDeviceUpdate %f us\n", deltaNs / 1000.0 / 1000.0,
            m_deviceUpdateTimer.GetUs());
    }

    m_updateTimer.Start();
}

void IOManager::__LoadIOLibs(const std::string& libPath, const std::vector<std::string>& libsToLoad,
        ks::Settings& settings)
{
    ks::FileSystem fs(libPath);

#ifdef IO_PLATFORM_WIN32
    const std::string libExt = ".dll";
#elif defined(IO_PLATFORM_LINUX)
    const std::string libExt = ".so";
#else
#error "Not valid platform selected"
#endif

    KS_LOG_INFO("Loading IO libraries (%d) from '%s'", libsToLoad.size(), fs.GetRootPath());

    for (const auto &libName : libsToLoad) {
        std::string path = fs.GetAbsolutePath(libName + libExt);

        KS_LOG_DEBUG("Loading IO lib (%s): %s", libName, path);

        try {
            auto lib = new ks::DynamicLibrary(path);
            io_device_instance_t func = (io_device_instance_t) lib->GetFuncAddr(io_device_instance_func_name);

            if (!func) {
                std::string name = lib->GetName();
                delete lib;
                throw IOException("Could not find entry point %s in library %s", io_device_instance_func_name, name);
            }

            auto device = new DeviceState(func(*m_ioManagerCtx));

            KS_LOG_INFO("Opening IODevice: %s", device->m_device->GetIdentifier());
            try {
                device->m_device->Open(settings);
            } catch (ks::Exception &e) {
                // assemble msg before deleting data that is used with the
                // message
                std::string identifier = device->m_device->GetIdentifier();
                delete device;

                delete lib;
                throw IOException("Opening device %s failed: %s", identifier, e.what());
            }

            m_ioLibs.push_back(lib);
            m_deviceStates.push_back(device);
        } catch (ks::Exception &e) {
            KS_LOG_ERROR("Loading IO lib failed: %s", e.what());
        }
    }
}

}