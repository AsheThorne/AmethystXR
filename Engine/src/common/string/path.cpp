// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "path.h"

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrPath::AxrPath() :
    AxrString() {
}

AxrPath::AxrPath(const char* path, AxrDynamicAllocator* dynamicAllocator) :
    AxrString(dynamicAllocator) {
    AxrString::buildFromCharString(path);
}

AxrPath::AxrPath(AxrPath&& src) noexcept :
    AxrString(std::move(src)) {
    move_internal(std::move(src));
}

AxrPath::~AxrPath() {
    cleanup();
}

AxrPath& AxrPath::operator=(const char* src) {
    AxrString::buildFromCharString(src);
    return *this;
}

AxrPath& AxrPath::operator=(AxrPath&& src) noexcept {
    if (this != &src) {
        cleanup();

        AxrString::operator=(std::move(src));

        move_internal(std::move(src));
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

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrPath::cleanup() {
    AxrString::cleanup();
}

void AxrPath::move_internal(AxrPath&& src) {
    // Please note that we aren't moving the base class. That should be done before calling this function because
    // depending on how it's done, it changes if we call the base move constructor or move assignment operator.
}
