#pragma once

#include <string>

#include "File.h"

namespace ks {

class FileMemory : public File
{
public:
    FileMemory(const std::string& path, File::Mode mode);

    ~FileMemory() override;

    size_t Read(void* buffer, size_t len) override;

    size_t Write(const void* buffer, size_t len) override;

    size_t Seek(ssize_t pos, bool absolute = true) override;

    size_t Pos() override;

    size_t Size() override;

    bool Eof() override;

private:
    void* m_buffer;
    size_t m_size;
    size_t m_pos;
};

}
