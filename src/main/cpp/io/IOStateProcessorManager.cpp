#include "IOStateProcessorManager.h"

#include "ks/Logger.hpp"

namespace io {

IOStateProcessorManager::~IOStateProcessorManager()
{
    for (auto& it : m_ioProcessors) {
        for (auto& it2 : it.second) {
            delete it2.second;
        }
    }
}

void IOStateProcessorManager::Add(InputStateProcessor* processor)
{
    auto it = m_ioProcessors.find(processor->GetDestIdent());

    if (it == m_ioProcessors.end()) {
        it = m_ioProcessors.insert(std::make_pair(processor->GetDestIdent(),
            std::unordered_map<std::string, IOStateProcessors*>())).first;
    }

    auto it2 = it->second.find(processor->GetSrcDeviceIdent());

    if (it2 == it->second.end()) {
        it2 = it->second.insert(std::make_pair(processor->GetSrcDeviceIdent(), new IOStateProcessors())).first;
    }

    if (it2->second->m_input) {
        KS_LOG_WARN("InputStateProcessor for %s -> %s is already assigned replacing", processor->GetSrcDeviceIdent(),
            processor->GetDestIdent());

        delete it2->second->m_input;
    }

    it2->second->m_input = processor;

    KS_LOG_DEBUG("Add Input SP: %s -> %s", processor->GetSrcDeviceIdent(), processor->GetDestIdent());
}

void IOStateProcessorManager::Add(OutputStateProcessor* processor)
{
    auto it = m_ioProcessors.find(processor->GetSrcIdent());

    if (it == m_ioProcessors.end()) {
        it = m_ioProcessors.insert(std::make_pair(processor->GetSrcIdent(),
            std::unordered_map<std::string, IOStateProcessors*>())).first;
    }

    auto it2 = it->second.find(processor->GetDestDeviceIdent());

    if (it2 == it->second.end()) {
        it2 = it->second.insert(std::make_pair(processor->GetDestDeviceIdent(), new IOStateProcessors())).first;
    }

    if (it2->second->m_output) {
        KS_LOG_WARN("OutputStateProcessor for %s -> %s is already assigned replacing", processor->GetSrcIdent(),
            processor->GetDestDeviceIdent());

        delete it2->second->m_output;
    }

    it2->second->m_output = processor;

    KS_LOG_DEBUG("Add Output SP: %s -> %s", processor->GetSrcIdent(), processor->GetDestDeviceIdent());
}

const std::unordered_map<std::string, IOStateProcessorManager::IOStateProcessors*>&
        IOStateProcessorManager::GetProcessors(const std::string& ioProcessorKey) const
{
    auto it = m_ioProcessors.find(ioProcessorKey);

    if (it == m_ioProcessors.end()) {
        // empty map, no processors available
        return m_emptyMap;
    }

    return it->second;
};

}