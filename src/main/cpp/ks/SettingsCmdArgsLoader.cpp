#include "SettingsCmdArgsLoader.h"

#include "StringUtils.h"

namespace ks {

SettingsCmdArgsLoader::SettingsCmdArgsLoader(int argc, char** argv) :
    m_argc(argc),
    m_argv(argv)
{

}

void SettingsCmdArgsLoader::Load(Settings& settings)
{
    for (int i = 0; i < m_argc; i++) {
        if (!strncmp(m_argv[i], "--", 2)) {
            size_t len = strlen(m_argv[i]);

            if (len == 2) {
                continue;
            }

            std::string key(m_argv[i] + 2, len - 2);
            key = ks::StringUtils::Trim(key, " \t");

            if (i + 1 < m_argc) {
                std::string val(m_argv[i + 1], strlen(m_argv[i + 1]));
                val = ks::StringUtils::Trim(val, " \t");

                i++;

                // check if there is an existing entry
                auto it = _GetStorage(settings).find(key);

                if (it != _GetStorage(settings).end()) {
                    // delete existing entry, i.e. overriding it with a new value
                    delete it->second;

                    _GetStorage(settings).erase(it);
                }

                Settings::Entry* entry;

                if (__IsInteger(val)) {
                    entry = new Settings::EntryInt(key, std::atol(val.c_str()));
                } else if (__IsFloat(val)) {
                    entry = new Settings::EntryFloat(key, std::atof(val.c_str()));
                } else if (__IsBool(val)) {
                    StringUtils::ToLower(val);
                    entry = new Settings::EntryBool(key, val == "true" || val == "yes");
                } else {
                    entry = new Settings::EntryString(key, val);
                }

                _GetStorage(settings).insert(std::make_pair(entry->GetKey(), entry));
            }
        }
    }
}

bool SettingsCmdArgsLoader::__IsInteger(const std::string& str)
{
    return std::find_if(str.begin(), str.end(),
        [](char c) {
            return !std::isdigit(c);
        }) == str.end();
}

bool SettingsCmdArgsLoader::__IsFloat(const std::string& str)
{
    return std::find_if(str.begin(), str.end(),
        [](char c) {
            return !std::isdigit(c) && c != '.';
        }) == str.end();
}

bool SettingsCmdArgsLoader::__IsBool(const std::string& str)
{
    std::string tmp = str;
    StringUtils::ToLower(tmp);
    return tmp == "true" || tmp == "yes" || tmp == "false" || tmp == "no";
}

}