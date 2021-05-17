#pragma once

#include <cstdint>

#include "ks/FileSystem.h"
#include "ks/Object.h"

namespace sgl {

class SaveState : public ks::Object
{
public:
    explicit SaveState(ks::FileSystem* fs);

    ~SaveState() override;

    uint32_t GetGameEntrySelected() {
        return m_data->m_gameEntrySelected;
    }

    void SetGameEntrySelected(size_t entry) {
        m_data->m_gameEntrySelected = static_cast<uint32_t>(entry);
    }

    void Save();

    void Load();

private:
    static const std::string ms_filename;

    struct Data
    {
        static const uint32_t ms_headerIdent = 0xA1B2C3D4;

        uint32_t m_header = ms_headerIdent;
        uint32_t m_gameEntrySelected = 0;
    };

private:
    ks::FileSystem* m_fs;
    Data* m_data;
};

}
