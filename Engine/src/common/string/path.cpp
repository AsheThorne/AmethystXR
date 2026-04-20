// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "path.h"
#include "../../platform/platform.h"

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrPath::AxrPath() :
    AxrString() {
}

AxrPath::AxrPath(AxrDynamicAllocator* dynamicAllocator) :
    AxrString(dynamicAllocator) {
}

AxrPath::AxrPath(AxrPath&& src) noexcept :
    AxrString(std::move(src)) {
    move_internal(std::move(src), true);
}

AxrPath::~AxrPath() {
    cleanup();
}

AxrPath& AxrPath::operator=(const char* src) {
    if (src == nullptr) {
        return *this;
    }

    AxrString::buildFromCharString(src);
    correctPathSeparators(AxrString::begin(), AxrString::end());
    return *this;
}

AxrPath& AxrPath::operator=(AxrPath&& src) noexcept {
    if (this != &src) {
        cleanup();

        AxrString::operator=(std::move(src));

        move_internal(std::move(src), false);
    }
    return *this;
}

bool AxrPath::operator==(const char8_t* srcString) const {
    return AxrString::operator==(srcString);
}

bool AxrPath::operator==(const char* srcString) const {
    return AxrString::compareWithCharString(srcString);
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrResult AxrPath::append(const AxrPath& path) {
    return AxrString::append(path.data());
}

AxrResult AxrPath::appendString(const AxrString& string) {
    return AxrString::append(string);
}

#define AXR_FUNCTION_FAILED_STRING "Failed to append AxrPath. "
AxrResult AxrPath::appendString(const char* string) {
    const size_t stringSize = std::char_traits<char>::length(string);
    const size_t currentStringSize = AxrString::size();

    const AxrResult axrResult = AxrString::growAllocation(currentStringSize + stringSize);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to grow allocation.");
        return axrResult;
    }

    AxrString::setStringData(string, stringSize, currentStringSize);

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

AxrResult AxrPath::appendString(const char8_t* string) {
    return AxrString::append(string);
}

AxrResult AxrPath::appendPath(const char8_t* path) {
    const AxrStringIterator oldEndPoint = AxrString::end();

    const AxrResult axrResult = appendString(path);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    correctPathSeparators(oldEndPoint, AxrString::end());

    return AXR_SUCCESS;
}

AxrResult AxrPath::appendPath(const char* path) {
    const AxrStringIterator oldEndPoint = AxrString::end();

    const AxrResult axrResult = appendString(path);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    correctPathSeparators(oldEndPoint, AxrString::end());

    return AXR_SUCCESS;
}

bool AxrPath::isAbsolute() const {
    return AxrPlatform::isPathAbsolute(*this);
}

bool AxrPath::isRelative() const {
    return AxrPlatform::isPathRelative(*this);
}

// ----------------------------------------- //
// Hidden Base Functions
// ----------------------------------------- //

AxrResult AxrPath::copy(const AxrString& string) {
    return AXR_ERROR_NOT_SUPPORTED;
}

AxrResult AxrPath::append(const AxrString& string) {
    return AXR_ERROR_NOT_SUPPORTED;
}

AxrResult AxrPath::append(const char8_t* string) {
    return AXR_ERROR_NOT_SUPPORTED;
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrPath::cleanup() {
    AxrString::cleanup();
}

void AxrPath::move_internal(AxrPath&& src, const bool useConstructor) {
    // Please note that we aren't moving the base class. That should be done before calling this function.
}

void AxrPath::correctPathSeparators(const size_t characterIndex, const size_t count) {
    Iterator startIterator = AxrString::end();

    size_t currentCharacterIndex = 0;
    for (Iterator beginIt = AxrString::begin(), endIt = AxrString::end(); beginIt != endIt; ++beginIt) {
        if (currentCharacterIndex == characterIndex) {
            startIterator = beginIt;
            break;
        }

        ++currentCharacterIndex;
    }

    correctPathSeparators(startIterator, count);
}

void AxrPath::correctPathSeparators(const Iterator& startIterator, const size_t count) {
    Iterator endIterator = AxrString::end();

    size_t currentCharacterIndex = 0;
    for (Iterator beginIt = startIterator, endIt = AxrString::end(); beginIt != endIt; ++beginIt) {
        if (currentCharacterIndex == count) {
            endIterator = beginIt;
            break;
        }

        ++currentCharacterIndex;
    }

    correctPathSeparators(startIterator, endIterator);
}

void AxrPath::correctPathSeparators(const Iterator& startIterator, const Iterator& endIterator) {
    for (Iterator beginIT = startIterator, endIT = endIterator; beginIT != endIT; ++beginIT) {
        AxrArray<char8_t, 4> currentCharacter = *beginIT;
        if ((currentCharacter == u8"/" || currentCharacter == u8"\\") && currentCharacter != m_Separator) {
            const size_t characterIndex = reinterpret_cast<uintptr_t>(beginIT.getDataPtr()) -
                                          reinterpret_cast<uintptr_t>(AxrString::begin().getDataPtr());

            /// All separators are in ascii. So they will all have the same size and we won't have issues by simply
            /// changing the character.
            if (AxrString::m_IsHeapAllocated) {
                AxrString::m_HeapString.Data[characterIndex] = m_Separator;
            } else {
                AxrString::m_StackString.Data[characterIndex] = m_Separator;
            }
        }
    }
}
