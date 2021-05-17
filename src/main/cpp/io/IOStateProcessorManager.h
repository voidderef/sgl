#pragma once

#include <unordered_map>

#include "InputStateProcessor.h"
#include "OutputStateProcessor.h"

namespace io {

class IOStateProcessorManager
{
public:
    struct IOStateProcessors {
        InputStateProcessor* m_input {};
        OutputStateProcessor* m_output {};

        IOStateProcessors() :
            m_input(nullptr),
            m_output(nullptr)
        {};

        ~IOStateProcessors() {
            if (m_input) {
                delete m_input;
            }

            if (m_output) {
                delete m_output;
            }
        }
    };

public:
    IOStateProcessorManager() = default;

    ~IOStateProcessorManager();

    void Add(InputStateProcessor* processor);

    void Add(OutputStateProcessor* processor);

    const std::unordered_map<std::string, IOStateProcessors*>& GetProcessors(
        const std::string& ioProcessorKey) const;

private:
    const std::unordered_map<std::string, IOStateProcessors*> m_emptyMap;

    std::unordered_map<std::string, std::unordered_map<std::string,
        IOStateProcessors*>> m_ioProcessors;
};

}
