// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "id.h"
#include "../debugInfo/debugInfo.h"
#include "../memory/allocator.h"

#include <xxhash.h>

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrID_T axrGenerateID(const char8_t* name, const size_t length) {
    return AxrID::generateID(name, length);
}

const char8_t* axrGetIDName(const AxrID_T id) {
    return AxrID::getIDName(id);
}

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrID::AxrID() = default;

AxrID::AxrID(const AxrID_T id) :
    m_ID(id) {
}

bool AxrID::operator==(const AxrID& other) const {
    return m_ID == other.m_ID;
}

bool AxrID::operator==(const AxrID_T other) const {
    return m_ID == other;
}

bool AxrID::operator!=(const AxrID& other) const {
    return !(*this == other);
}

bool AxrID::operator!=(const AxrID_T other) const {
    return !(*this == other);
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrID_T AxrID::get() const {
    return m_ID;
}

AxrID_T AxrID::generateID(const char8_t* name, const size_t length) {
    const AxrID_T id = XXH3_64bits(name, length);
#ifdef AXR_DEBUG_INFO_ENABLED
    AxrDebugInfo::get().IDNames.insert(AxrID(id), AxrString(name, &AxrAllocator::get().DebugInfoAllocator));
#endif
    return id;
}

const char8_t* AxrID::getIDName(const AxrID_T id) {
#ifdef AXR_DEBUG_INFO_ENABLED
    const AxrUnorderedMap_Dynamic<AxrID, AxrString>& idNames = AxrDebugInfo::get().IDNames;
    const auto iterator = idNames.find(AxrID(id));
    if (iterator != idNames.end()) {
        return (*iterator).second.data();
    }
#endif
    return u8"";
}
