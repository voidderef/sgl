#pragma once

#include <string>

namespace ks {

class StringUtils
{
public:
    static const std::string Trim(const std::string& str, const std::string& trim_chars);

    static std::string ToLower(const std::string& str);

    static void ToLower(std::string& str);

    static std::string ToUpper(const std::string& str);

    static void ToUpper(std::string& str);

    static std::string BufferAsHex(const uint8_t* buffer, size_t length, uint32_t lineSize = 0, bool spaces = true);

private:
    StringUtils() = default;

    ~StringUtils() = default;
};

}
