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

std::wstring axrToWString(const char* string) {
    const size_t stringLength = strlen(string) + 1;
    size_t numOfConvertedChars;
    std::wstring convertedChars(stringLength, L'\0');

    const errno_t error = mbstowcs_s(
        &numOfConvertedChars,
        convertedChars.data(),
        stringLength,
        string,
        stringLength - 1
    );

    if (error != 0) {
        axrLogErrorLocation("mbstowcs_s() failed for string: {0}", string);
        return L"";
    }

    return convertedChars;
}

bool axrContainsString(
    const char* string,
    const std::vector<std::string>& stringCollection
) {
    for (const std::string& str : stringCollection) {
        if (std::strcmp(string, str.c_str()) == 0) {
            return true;
        }
    }

    return false;
}
