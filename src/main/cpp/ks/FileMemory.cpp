#include "FileMemory.h"

#include <cstdio>

#include "FileException.h"

namespace ks {

FileMemory::FileMemory(const std::string& path, File::Mode mode) :
    File(path, mode),
    m_buffer(nullptr),
    m_size(0),
    m_pos(0)
{
    if (mode == File::e_ModeCreate) {
        throw FileException("Creating in memory file not supported");
    }

    FILE* file = fopen(path.c_str(), "r");

    if (!file) {
        throw FileException(errno, "Opening file '%s' failed", path.c_str());
    }

    fseek(file, 0, SEEK_END);
    m_size = static_cast<size_t>(ftell(file));
    fseek(file, 0, SEEK_SET);

    m_buffer = malloc(m_size);

    if (fread(m_buffer, m_size, 1, file) != 1) {
        throw FileException(errno, "Reading file '%s' into memory failed", path.c_str());
    }

    fclose(file);
}

FileMemory::~FileMemory()
{
    free(m_buffer);
}

size_t FileMemory::Read(void* buffer, size_t len)
{
    size_t toRead = len;

    if (m_pos == m_size) {
        return 0;
    }

    if (m_pos + toRead >= m_size) {
        toRead = m_size - m_pos;
    }

    memcpy(buffer, (void*) (((uintptr_t) m_buffer) + m_pos), toRead);

    m_pos += toRead;
    return toRead;
}

size_t FileMemory::Write(const void* buffer, size_t len)
{
    size_t toWrite = len;

    if (m_pos == m_size) {
        return 0;
    }

    if (m_pos + toWrite >= m_size) {
        toWrite = m_size - m_pos;
    }

    memcpy((void*) (((uintptr_t) m_buffer) + m_pos), buffer, toWrite);

    m_pos += toWrite;
    return toWrite;
}

size_t FileMemory::Seek(ssize_t pos, bool absolute)
{
    if (absolute) {
        if (pos < 0) {
            throw FileException("Invalid absolute seek with pos < 0: %d", pos);
        }

        if (pos >= m_size) {
            m_pos = m_size;
        } else {
            m_pos = static_cast<size_t>(pos);
        }
    } else {
        if (pos > 0) {
            if (m_pos + pos >= m_size) {
                m_pos = m_size;
            } else {
                m_pos += pos;
            }
        } else {
            ssize_t tmp = m_pos;

            if (tmp + pos < 0) {
                m_pos = 0;
            } else {
                m_pos = static_cast<size_t>(tmp - pos);
            }
        }
    }

    return m_pos;
}

size_t FileMemory::Pos()
{
    return m_pos;
}

size_t FileMemory::Size()
{
    return m_size;
}

bool FileMemory::Eof()
{
    return m_pos >= m_size;
}

}