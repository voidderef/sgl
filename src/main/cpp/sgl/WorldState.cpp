#include "WorldState.h"

#include "ks/Logger.hpp"

namespace sgl {

WorldState::WorldState(const std::string& name) :
    m_name(name)
{

}

WorldState::~WorldState()
{
    Clear();
}

void WorldState::Add(const std::string& name, ks::Object* obj)
{
    if (!obj) {
        throw ks::Exception("Adding object with name '" + name + "' to world state '" + m_name + "' failed, nullptr");
    }

    auto it = m_objects.insert(std::make_pair(name, obj));

    if (!it.second) {
        throw ks::Exception("Cannot add object with name '" + name + "' to world state '" + m_name +
            "', name entry already used");
    }
}

void WorldState::Replace(const std::string& name, ks::Object* obj)
{
    if (!obj) {
        throw ks::Exception("Replacing object with name '" + name + "' in world state '" + m_name +
            "' failed, nullptr");
    }

    auto it = m_objects.find(name);

    if (it != m_objects.end()) {
        delete it->second;
        m_objects.erase(it);
    }

    m_objects.insert(std::make_pair(name, obj));
}

void WorldState::Remove(const std::string& name)
{
    auto it = m_objects.find(name);

    if (it != m_objects.end()) {
        delete it->second;
        m_objects.erase(it);
    }
}

void WorldState::Clear()
{
    for (auto& it : m_objects) {
        KS_LOG_DEBUG("[%s] Delete '%s'", m_name, it.first);
        delete it.second;
    }

    m_objects.clear();
}

}