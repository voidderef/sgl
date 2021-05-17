#pragma once

#include "ks/Exception.h"

namespace sgl {

class TextureException : public ks::Exception
{
public:
    explicit TextureException(const std::string& msg) :
        Exception(msg)
    {

    }

    ~TextureException() override {};
};

}
