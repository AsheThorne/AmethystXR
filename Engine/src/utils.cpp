// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "utils.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <cstdlib>

// ---- String Utils ----

std::wstring axrToWString(const std::string& string) {
    size_t numOfConvertedChars;
    std::wstring convertedChars(string.size(), L'\0');

    const errno_t error = mbstowcs_s(
        &numOfConvertedChars,
        convertedChars.data(),
        // + 1 to include the null character
        string.size() + 1,
        string.c_str(),
        string.size()
    );

    if (error != 0) {
        axrLogErrorLocation("mbstowcs_s() failed for string: {0}", string.c_str());
        return L"";
    }

    return convertedChars;
}

bool axrContainsString(
    const std::string& string,
    const std::vector<std::string>& stringCollection
) {
    for (const std::string& str : stringCollection) {
        if (string == str) {
            return true;
        }
    }

    return false;
}

bool axrStringIsEmpty(const char* string) {
    return string == nullptr || strcmp(string, "") == 0;
}
