#include "SaveState.h"

#include "ks/Logger.hpp"

namespace sgl {

const std::string SaveState::ms_filename = "sgl.save";

SaveState::SaveState(ks::FileSystem* fs) :
    m_fs(fs),
    m_data(new Data())
{

}

SaveState::~SaveState()
{
    delete m_fs;
    delete m_data;
}

void SaveState::Save()
{
    if (!m_fs->Exists()) {
        KS_LOG_ERROR("Can't save save state to path %s, does not exist", m_fs->GetRootPath());
    } else {
        m_fs->SaveFile(ms_filename, m_data, sizeof(Data));
    }
}

void SaveState::Load()
{
    size_t size = 0;

    if (!m_fs->Exists(ms_filename)) {
        KS_LOG_ERROR("Can't load save state %s from path %s, does not exist", ms_filename, m_fs->GetRootPath());
        return;
    }

    delete m_data;
    m_data = static_cast<Data*>(m_fs->LoadFile(ms_filename, &size));

    if (size != sizeof(Data)) {
        KS_LOG_ERROR("Loading save state %s from directory %s failed, corrupted data, using default values",
            ms_filename, m_fs->GetRootPath());
        m_data = new Data();
    }

    if (m_data->m_header != Data::ms_headerIdent) {
        KS_LOG_ERROR("Corrupted file header of save state %s from directory %s, using default values", ms_filename,
            m_fs->GetRootPath());
        delete m_data;
        m_data = new Data();
    }
}

}