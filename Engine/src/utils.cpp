// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "utils.hpp"
#include "axr/logger.h"
#include "axr/common/utils.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <cstdlib>

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrStringIsEmpty(const char* string) {
    return string == nullptr || strcmp(string, "") == 0;
}

char* axrCloneString(const char* string) {
    if (axrStringIsEmpty(string)) {
        return nullptr;
    }

    char* newString = new char[strlen(string) + 1]{};
    strcpy_s(newString, strlen(string) + 1, string);
    return newString;
}

void axrDestroyString(char** string) {
    if (string == nullptr || axrStringIsEmpty(*string)) {
        return;
    }

    delete[] *string;
    *string = nullptr;
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

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
        return {};
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
