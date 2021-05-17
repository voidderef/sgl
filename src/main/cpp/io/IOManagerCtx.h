#pragma once

#include <unordered_map>

#include "ks/Object.h"

namespace io {

class IOManagerCtx : public ks::Object
{
public:
    IOManagerCtx() {}

    ~IOManagerCtx() override {}

    template<typename V> void Put(const std::string& key, V* value)
    {
        m_ctx.insert(std::make_pair(key, value));
    }

    template<typename V> V Get(const std::string& key) const
    {
        return reinterpret_cast<V>(m_ctx.at(key));
    }

private:
    std::unordered_map<std::string, void*> m_ctx;
};

}
