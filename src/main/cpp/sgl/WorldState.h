#pragma once

#include <string>
#include <unordered_map>

#include "ks/Exception.h"

#include "ks/Object.h"

namespace sgl {

// probably not the best name. our huge state object which is passed from
// screen to screen containing everything that needs to be shared across
// screens
class WorldState
{
public:
    explicit WorldState(const std::string& name);
    ~WorldState();

    const std::string& GetName() const {
        return m_name;
    }

    void Add(const std::string& name, ks::Object* obj);

    void Replace(const std::string& name, ks::Object* obj);

    template<class T>
    typename std::enable_if<std::is_base_of<ks::Object, T>::value, T*>::type
    Get(const std::string& name) const
    {
        auto it = m_objects.find(name);

        if (it == m_objects.end()) {
            throw ks::Exception("Cannot find object '" + name +
                "' in world state '" + m_name + "'");
        }

        return (T*) it->second;
    }

    template<class T>
    typename std::enable_if<std::is_base_of<ks::Object, T>::value, T*>::type
    GetAndRemove(const std::string& name)
    {
        T* obj = nullptr;

        auto it = m_objects.find(name);

        if (it == m_objects.end()) {
            throw ks::Exception("Cannot find object '" + name +
                "' in world state '" + m_name + "'");
        }

        obj = (T*) it->second;
        m_objects.erase(it);

        return obj;
    }

    void Remove(const std::string& name);

    void Clear();

private:
    const std::string m_name;
    std::unordered_map<std::string, ks::Object*> m_objects;
};

}
