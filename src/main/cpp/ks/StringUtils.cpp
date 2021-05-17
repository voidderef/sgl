#include "StringUtils.h"

#include <algorithm>
#include <iomanip>

namespace ks {

const std::string StringUtils::Trim(const std::string& str,
        const std::string& trim_chars)
{
    std::string str_trimmed = str;
    str_trimmed.erase(0, str_trimmed.find_first_not_of(trim_chars));
    str_trimmed.erase(str_trimmed.find_last_not_of(trim_chars) + 1U);

    return str_trimmed;
}

std::string StringUtils::ToLower(const std::string& str)
{
    std::string strNew = str;
    std::transform(strNew.begin(), strNew.end(), strNew.begin(), ::tolower);
    return strNew;
}

void StringUtils::ToLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

std::string StringUtils::ToUpper(const std::string& str)
{
    std::string strNew = str;
    std::transform(strNew.begin(), strNew.end(), strNew.begin(), ::toupper);
    return strNew;
}

void StringUtils::ToUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

std::string StringUtils::BufferAsHex(const uint8_t* buffer, size_t length,
        uint32_t lineSize, bool spaces)
{
    std::ostringstream os;

    for (size_t i = 0; i < length; i++) {
        os << std::setfill('0') << std::setw(2) << std::hex << (uint16_t) buffer[i];

        if (lineSize != 0 && (i + 1) % lineSize == 0 && i != 0) {
            os << "\n";
        } else if (spaces) {
            os << " ";
        }
    }

    return os.str();
}

}