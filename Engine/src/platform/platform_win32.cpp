// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "platform.h"

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

bool AxrPlatform::isPathAbsolute(const AxrPath& path) {
    AxrPath::Iterator currentCharIt = path.begin();
    const AxrPath::Iterator endIt = path.end();

    if (currentCharIt == endIt) {
        // The path is empty
        return false;
    }

    if (*currentCharIt == u8"\\") {
        if (++currentCharIt == endIt) {
            // The path is a single backslash
            return false;
        }

        if (*currentCharIt == u8"\\") {
            // The path is a double backslash. Meaning it's on a network drive. Which is a valid absolute path
            return true;
        }

        return false;
    }

    AxrArray<char8_t, 4> driveLetter = *currentCharIt;
    if (driveLetter[0] >= 'A' && driveLetter[0] <= 'Z') {
        if (++currentCharIt == endIt) {
            // Path is a single letter
            return false;
        }

        if (*currentCharIt != u8":") {
            // The first character is not a drive letter
            return false;
        }

        if (++currentCharIt == endIt) {
            // Path is a single drive letter and colon, missing the backslash
            return false;
        }

        if (*currentCharIt == u8"\\") {
            // Path is a drive letter, followed by a colon and backslash. E.g. "C:/".
            return true;
        }

        return false;
    }

    return false;
}
