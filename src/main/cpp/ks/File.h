#pragma once

#include <cstdlib>
#include <ostream>
#include <string>
#include <utility>

namespace ks {

class File
{
public:
    enum Mode {
        e_ModeRead = 0,
        e_ModeWrite = 1,
        e_ModeCreate = 2,
    };

public:
    File(std::string name, Mode mode) :
        m_name(std::move(name)),
        m_mode(mode)
    {
    }

    virtual ~File() = default;

    const std::string& GetName() const {
        return m_name;
    }

    Mode GetMode() const {
        return m_mode;
    }

    virtual size_t Read(void* buffer, size_t len) = 0;

    virtual size_t Write(const void* buffer, size_t len) = 0;

    virtual size_t Seek(ssize_t pos, bool absolute = true) = 0;

    virtual size_t Pos() = 0;

    virtual size_t Size() = 0;

    virtual bool Eof() = 0;

    friend std::ostream& operator<<(std::ostream& ostream, const File& f) {
         ostream << f.GetName();
         return ostream;
    }

private:
    const std::string m_name;
    const Mode m_mode;
};

}
