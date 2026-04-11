// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "id.h"

#include <xxhash.h>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrID::AxrID(const uint64_t id) :
    m_ID(id) {
}

uint64_t AxrID::get() const {
    return m_ID;
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

uint64_t AxrID::generateID(const char* string, const size_t length) {
    return XXH3_64bits(string, length);
}

// ---------------------------------------------------------------------------------- //
//                               User Defined Literals                                //
// ---------------------------------------------------------------------------------- //

uint64_t operator""_id(const char* string, const size_t length) {
    return AxrID::generateID(string, length);
}
