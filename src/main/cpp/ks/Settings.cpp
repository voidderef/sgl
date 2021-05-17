#include "Settings.h"

namespace ks {

const std::string Settings::ms_delim = "/";

Settings::Settings()
{

}

Settings::~Settings()
{
    for (auto& it : m_storage) {
        delete it.second;
    }
}

template<>
int64_t Settings::GetValue(const std::string& key) const
{
    auto ret = __GetValueInt(key);

    if (ret > std::numeric_limits<int64_t>::max()) {
        throw SettingsException("Value %lu of key %s exceeds limit %lu", ret, key,
            std::numeric_limits<int64_t>::max());
    }

    if (ret < std::numeric_limits<int64_t>::min()) {
        throw SettingsException("Value %lu of key %s deceeds limit %lu", ret, key,
            std::numeric_limits<int64_t>::min());
    }

    return static_cast<int64_t>(ret);
}

template<>
int32_t Settings::GetValue(const std::string& key) const
{
    auto ret = __GetValueInt(key);

    if (ret > std::numeric_limits<int32_t>::max()) {
        throw SettingsException("Value %lu of key %s exceeds limit %lu", ret, key,
            std::numeric_limits<int32_t>::max());
    }

    if (ret < std::numeric_limits<int64_t>::min()) {
        throw SettingsException("Value %lu of key %s deceeds limit %lu", ret, key,
            std::numeric_limits<int32_t>::min());
    }

    return static_cast<int32_t>(ret);
}

template<>
int16_t Settings::GetValue(const std::string& key) const
{
    auto ret = __GetValueInt(key);

    if (ret > std::numeric_limits<int16_t>::max()) {
        throw SettingsException("Value %lu of key %s exceeds limit %lu", ret, key,
            std::numeric_limits<int16_t>::max());
    }

    if (ret < std::numeric_limits<int16_t>::min()) {
        throw SettingsException("Value %lu of key %s deceeds limit %lu", ret, key,
            std::numeric_limits<int16_t>::min());
    }

    return static_cast<int16_t>(ret);
}

template<>
int8_t Settings::GetValue(const std::string& key) const
{
    auto ret = __GetValueInt(key);

    if (ret > std::numeric_limits<int8_t>::max()) {
        throw SettingsException("Value %lu of key %s exceeds limit %lu", ret, key,
            std::numeric_limits<int8_t>::max());
    }

    if (ret < std::numeric_limits<int8_t>::min()) {
        throw SettingsException("Value %lu of key %s deceeds limit %lu", ret, key,
            std::numeric_limits<int8_t>::min());
    }

    return static_cast<int8_t>(ret);
}

template<>
uint64_t Settings::GetValue(const std::string& key) const
{
    auto ret = __GetValueInt(key);

    if (ret > std::numeric_limits<uint64_t>::max()) {
        throw SettingsException("Value %lu of key %s exceeds limit %lu", ret, key,
            std::numeric_limits<uint64_t>::max());
    }

    if (ret < std::numeric_limits<uint64_t>::min()) {
        throw SettingsException("Value %lu of key %s deceeds limit %lu", ret, key,
            std::numeric_limits<uint64_t>::min());
    }

    return static_cast<uint64_t>(ret);
}

template<>
uint32_t Settings::GetValue(const std::string& key) const
{
    auto ret = __GetValueInt(key);

    if (ret > std::numeric_limits<uint32_t>::max()) {
        throw SettingsException("Value %lu of key %s exceeds limit %lu", ret, key,
            std::numeric_limits<uint32_t>::max());
    }

    if (ret < std::numeric_limits<uint32_t>::min()) {
        throw SettingsException("Value %lu of key %s deceeds limit %lu", ret, key,
            std::numeric_limits<uint32_t>::min());
    }

    return static_cast<uint32_t>(ret);
}

template<>
uint16_t Settings::GetValue(const std::string& key) const
{
    auto ret = __GetValueInt(key);

    if (ret > std::numeric_limits<uint16_t>::max()) {
        throw SettingsException("Value %lu of key %s exceeds limit %lu", ret, key,
            std::numeric_limits<uint16_t>::max());
    }

    if (ret < std::numeric_limits<uint16_t>::min()) {
        throw SettingsException("Value %lu of key %s deceeds limit %lu", ret, key,
            std::numeric_limits<uint16_t>::min());
    }

    return static_cast<uint16_t>(ret);
}

template<>
uint8_t Settings::GetValue(const std::string& key) const
{
    auto ret = __GetValueInt(key);

    if (ret > std::numeric_limits<uint8_t>::max()) {
        throw SettingsException("Value %lu of key %s exceeds limit %lu", ret, key,
            std::numeric_limits<uint8_t>::max());
    }

    if (ret < std::numeric_limits<uint8_t>::min()) {
        throw SettingsException("Value %lu of key %s deceeds limit %lu", ret, key,
            std::numeric_limits<uint8_t>::min());
    }

    return static_cast<uint8_t>(ret);
}

template<>
double Settings::GetValue<double>(const std::string& key) const
{
    auto it = m_storage.find(key);

    if (it == m_storage.end()) {
        throw SettingsException("No value for key %s", key);
    }

    if (it->second->GetType() != Entry::e_TypeFloat) {
        throw SettingsException("Invalid type %d for key %s, requested type float", it->second->GetType(), key);
    }

    return static_cast<EntryFloat*>(it->second)->GetValue();
}

template<>
float Settings::GetValue<float>(const std::string& key) const
{
    auto ret = GetValue<double>(key);

    if (ret > std::numeric_limits<float>::max()) {
        throw SettingsException("Value %lu of key %s exceeds limit %lu", ret, key,
            std::numeric_limits<float>::max());
    }

    if (ret < std::numeric_limits<float>::min()) {
        throw SettingsException("Value %lu of key %s deceeds limit %lu", ret, key,
            std::numeric_limits<float>::min());
    }

    return static_cast<float>(ret);
}

template<>
const std::string& Settings::GetValue<const std::string&>(const std::string& key) const
{
    auto it = m_storage.find(key);

    if (it == m_storage.end()) {
        throw SettingsException("No value for key %s", key);
    }

    if (it->second->GetType() != Entry::e_TypeString) {
        throw SettingsException("Invalid type %d for key %s, requested type string", it->second->GetType(), key);
    }

    return static_cast<EntryString*>(it->second)->GetValue();
}

template<>
const std::vector<std::string>& Settings::GetValue<const std::vector<std::string>&>(const std::string& key) const
{
    auto it = m_storage.find(key);

    if (it == m_storage.end()) {
        throw SettingsException("No value for key %s", key);
    }

    if (it->second->GetType() != Entry::e_TypeStringList) {
        throw SettingsException("Invalid type %d for key %s, requested type string list", it->second->GetType(), key);
    }

    return static_cast<EntryStringList*>(it->second)->GetValue();
}

template<>
bool Settings::GetValue<bool>(const std::string& key) const
{
    auto it = m_storage.find(key);

    if (it == m_storage.end()) {
        throw SettingsException("No value for key %s", key);
    }

    if (it->second->GetType() != Entry::e_TypeBool) {
        throw SettingsException("Invalid type %d for key %s, requested type bool", it->second->GetType(), key);
    }

    return static_cast<EntryBool*>(it->second)->GetValue();
}

template<>
void Settings::SetValue<double>(const std::string& key, double value)
{
    auto it = m_storage.find(key);

    if (it != m_storage.end()) {
        if (it->second->GetType() != Entry::e_TypeFloat) {
            throw SettingsException("Invalid type %d (expected float) for existing key %s", it->second->GetType(), key);
        }

        m_storage.erase(it);
    }

    m_storage.insert(std::make_pair(key, new EntryFloat(key, value)));
}

template<>
void Settings::SetValue<float>(const std::string& key, float value)
{
    SetValue<double>(key, value);
}

template<>
void Settings::SetValue<const std::string&>(const std::string& key, const std::string& value)
{
    auto it = m_storage.find(key);

    if (it != m_storage.end()) {
        if (it->second->GetType() != Entry::e_TypeString) {
            throw SettingsException("Invalid type %d (expected string) for existing key %s",
                it->second->GetType(), key);
        }

        m_storage.erase(it);
    }

    m_storage.insert(std::make_pair(key, new EntryString(key, value)));
}

template<>
void Settings::SetValue<bool>(const std::string& key, bool value)
{
    auto it = m_storage.find(key);

    if (it != m_storage.end()) {
        if (it->second->GetType() != Entry::e_TypeBool) {
            throw SettingsException("Invalid type %d (expected bool) for existing key %s", it->second->GetType(), key);
        }

        m_storage.erase(it);
    }

    m_storage.insert(std::make_pair(key, new EntryBool(key, value)));
}

int64_t Settings::__GetValueInt(const std::string& key) const
{
    auto it = m_storage.find(key);

    if (it == m_storage.end()) {
        throw SettingsException("No value for key %s", key);
    }

    if (it->second->GetType() != Entry::e_TypeInt) {
        throw SettingsException("Invalid type %d for key %s, requested type int", it->second->GetType(), key);
    }

    return static_cast<EntryInt*>(it->second)->GetValue();
}

void Settings::__SetValueInt(const std::string& key, int64_t val)
{
    auto it = m_storage.find(key);

    if (it != m_storage.end()) {
        if (it->second->GetType() != Entry::e_TypeInt) {
            throw SettingsException("Invalid type %d (expected int) for existing key %s", it->second->GetType(), key);
        }

        m_storage.erase(it);
    }

    m_storage.insert(std::make_pair(key, new EntryInt(key, val)));
}

}