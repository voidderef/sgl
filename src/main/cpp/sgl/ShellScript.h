#pragma once

#include <string>

namespace sgl {

class ShellScript
{
public:
    explicit ShellScript(const std::string& path);

    ~ShellScript() = default;

    const std::string& GetPath() const {
        return m_path;
    }

    void Execute();

    friend std::ostream& operator<<(std::ostream& os, const ShellScript& o)
    {
        return os << o.m_path;
    }

private:
    const std::string m_path;
};

}
