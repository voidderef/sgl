#include "FileStream.h"

#include "FileException.h"

namespace ks {

FileStream::FileStream(const std::string& path, File::Mode mode) :
    File(path, mode),
    m_handle(nullptr),
    m_size(0),
    m_pos(0)
{
    const char* openMode;

    switch (mode) {
        case File::e_ModeRead:
            openMode = "r";
            break;

        case File::e_ModeWrite:
            openMode = "w";
            break;

        case File::e_ModeCreate:
            openMode = "w+";
            break;

        default:
            throw FileException("Invalid mode parameter %d specified", mode);

    }

    m_handle = fopen(path.c_str(), openMode);

    if (!m_handle) {
        throw FileException(errno, "Opening file '%s' failed", path.c_str());
    }

    fseek(m_handle, 0, SEEK_END);
    m_size = static_cast<size_t>(ftell(m_handle));
    fseek(m_handle, 0, SEEK_SET);
}

FileStream::~FileStream()
{
    fclose(m_handle);
}

size_t FileStream::Read(void* buffer, size_t len)
{
    size_t toRead = len;

    if (m_pos == m_size) {
        return 0;
    }

    if (m_pos + toRead >= m_size) {
        toRead = m_size - m_pos;
    }

    size_t read = fread(buffer, toRead, 1, m_handle);

    if (read != 1) {
        throw FileException(ferror(m_handle), "Reading file failed");
    } else {
        m_pos += toRead;
        return toRead;
    }
}

size_t FileStream::Write(const void* buffer, size_t len)
{
    size_t toWrite = len;

    if (m_pos == m_size) {
        return 0;
    }

    if (m_pos + toWrite >= m_size) {
        toWrite = m_size - m_pos;
    }

    size_t written = fwrite(buffer, toWrite, 1, m_handle);

    if (written != 1) {
        throw FileException(ferror(m_handle), "Writing file failed");
    } else {
        m_pos += toWrite;
        return toWrite;
    }
}

size_t FileStream::Seek(ssize_t pos, bool absolute)
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

    fseek(m_handle, m_pos, SEEK_SET);

    return m_pos;
}

size_t FileStream::Pos()
{
    return m_pos;
}

size_t FileStream::Size()
{
    return m_size;
}

bool FileStream::Eof()
{
    return m_pos >= m_size;
}

}