#pragma once

#include <string>
#include <utility>

#include "ks/Object.h"

namespace sgl {

class ErrorCtx : public ks::Object
{
public:
    explicit ErrorCtx(std::string message) :
        m_message(std::move(message))
    {}

    ~ErrorCtx() override = default;

    const std::string& GetMessage() const {
        return m_message;
    }

private:
    const std::string m_message;
};

}
