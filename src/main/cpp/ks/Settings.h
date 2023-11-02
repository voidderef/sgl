#pragma once

#include <limits>

#include "SettingsException.h"

namespace ks {

class SettingsLoader;
class SettingsSaver;

class Settings
{
public:
    friend class SettingsLoader;
    friend class SettingsSaver;

public:
    static const std::string ms_delim;

    Settings();

    ~Settings();

    template<typename T>
    T GetValue(const std::string& key) const;

    template<typename T>
    void SetValue(const std::string& key, T value)
    {
        __SetValueInt(key, static_cast<int64_t>(value));
    }

    std::string ToString() const;

    friend std::ostream& operator<<(std::ostream& os, const Settings& o)
    {
        for (auto& it : o.m_storage) {
            os << *it.second << std::endl;
        }

        return os;
    }

// needs to be public for loaders/savers
public:
    class Entry
    {
    public:
        enum Type {
            e_TypeInt,
            e_TypeFloat,
            e_TypeString,
            e_TypeBool,
            e_TypeStringList,
        };

    public:
        Entry(Type type, std::string key) :
            m_type(type),
            m_key(std::move(key))
        {}

        virtual ~Entry() = default;

        Type GetType() const {
            return m_type;
        }

        const std::string& GetKey() const {
            return m_key;
        }

        virtual std::string ToString() const = 0;

        friend std::ostream& operator<<(std::ostream& os, const Entry& o)
        {
            return os << o.ToString();
        }

    private:
        const Type m_type;
        const std::string m_key;
    };

    class EntryInt : public Entry
    {
    public:
        friend class Entry;

        EntryInt(const std::string& key, int64_t val) :
            Entry(e_TypeInt, key),
            m_val(val)
        {}

        ~EntryInt() override = default;

        int64_t GetValue() const {
            return m_val;
        }

        std::string ToString() const override {
            return "[int] " + GetKey() + " = " + std::to_string(GetValue());
        }

    private:
        int64_t m_val;
    };

    class EntryFloat : public Entry
    {
    public:
        friend class Entry;

        EntryFloat(const std::string& key, double val) :
            Entry(e_TypeFloat, key),
            m_val(val)
        {}

        ~EntryFloat() override = default;

        double GetValue() const {
            return m_val;
        }

        std::string ToString() const override {
            return "[float] " + GetKey() + " = " + std::to_string(GetValue());
        }

    private:
        double m_val;
    };

    class EntryString : public Entry
    {
    public:
        friend class Entry;

        EntryString(const std::string& key, std::string val) :
            Entry(e_TypeString, key),
            m_val(std::move(val))
        {}

        ~EntryString() override = default;

        const std::string& GetValue() const {
            return m_val;
        }

        std::string ToString() const override {
            return "[str] " + GetKey() + " = " + GetValue();
        }

    private:
        const std::string m_val;
    };

    class EntryBool : public Entry
    {
    public:
        friend class Entry;

        EntryBool(const std::string& key, bool val) :
            Entry(e_TypeBool, key),
            m_val(val)
        {}

        ~EntryBool() override = default;

        bool GetValue() const {
            return m_val;
        }

        std::string ToString() const override {
            if (GetValue()) {
                return "[bool] " + GetKey() + " = true";
            } else {
                return "[bool] " + GetKey() + " = false";
            }
        }

    private:
        bool m_val;
    };

    class EntryStringList : public Entry
    {
    public:
        friend class Entry;

        EntryStringList(const std::string& key, std::vector<std::string> value) :
            Entry(e_TypeStringList, key),
            m_val(std::move(value))
        {}

        ~EntryStringList() override = default;

        const std::vector<std::string>& GetValue() const {
            return m_val;
        }

        std::string ToString() const override {
            std::string str = "[str-list]";

            for (auto& it : m_val) {
                str += " " + it;
            }

            return str;
        }

    private:
        std::vector<std::string> m_val;
    };

private:
    std::unordered_map<std::string, Entry*> m_storage;

    int64_t __GetValueInt(const std::string& key) const;
    void __SetValueInt(const std::string& key, int64_t val);
};

}
