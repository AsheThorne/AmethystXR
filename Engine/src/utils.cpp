// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
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

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ---- Vulkan Utils ----

bool axrVkSucceeded(vk::Result result) {
    return VK_SUCCEEDED(static_cast<VkResult>(result));
}

bool axrVkFailed(vk::Result result) {
    return VK_FAILED(static_cast<VkResult>(result));
}

void axrLogVkResult(
    const vk::Result result,
    const char* functionName,
    const std::source_location& location
) {
    if (axrVkFailed(result)) {
        axrLogErrorLocation(
            {"{0} failed with a result of {1}.", location},
            functionName,
            to_string(result).c_str()
        );
    }
}

#endif
