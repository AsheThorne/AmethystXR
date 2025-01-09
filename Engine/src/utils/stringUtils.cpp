// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "stringUtils.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <cstdlib>


std::wstring axrToWString(const char* string) {
    const size_t stringLength = strlen(string) + 1;
    size_t numOfConvertedChars;
    std::wstring convertedChars(stringLength, L'#');
    
    const errno_t error = mbstowcs_s(
        &numOfConvertedChars,
        convertedChars.data(),
        stringLength,
        string,
        stringLength - 1
    );

    if (error != 0) {
        axrLogError("axrToWString - mbstowcs_s() failed for string: {0}", string);
        return L"";
    }

    return convertedChars;
}
