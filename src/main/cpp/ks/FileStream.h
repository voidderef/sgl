#pragma once

#include <cstdio>
#include <string>

#include "File.h"

namespace ks {

class FileStream : public File
{
public:
    FileStream(const std::string& path, File::Mode mode);

    ~FileStream() override;

    size_t Read(void* buffer, size_t len) override;

    size_t Write(const void* buffer, size_t len) override;

    size_t Seek(ssize_t pos, bool absolute) override;

    size_t Pos() override;

    size_t Size() override;

    bool Eof() override;

private:
    FILE* m_handle;
    size_t m_size;
    size_t m_pos;
};

}
