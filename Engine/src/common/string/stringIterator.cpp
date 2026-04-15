// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "stringIterator.h"
#include "axr/logging.h"

AxrStringIterator::AxrStringIterator(const char8_t* character) :
    m_Character(character) {
}

AxrStringIterator& AxrStringIterator::operator++() {
    if ((*m_Character & 0b10000000) == 0) {
        // Current character is a 1 byte code point (ascii)
        ++m_Character;
        return *this;
    }

    if (*m_Character >> 5 == 0b00000110) {
        // Current character is a 2 byte code point
        m_Character += 2;
        return *this;
    }

    if (*m_Character >> 4 == 0b00001110) {
        // Current character is a 3 byte code point
        m_Character += 3;
        return *this;
    }

    if (*m_Character >> 3 == 0b00011110) {
        // Current character is a 4 byte code point
        m_Character += 4;
        return *this;
    }

    axrLogError("Failed to increment AxrStringIterator. Current character is not a UTF-8 leading character.");
    return *this;
}

AxrStringIterator AxrStringIterator::operator++(int) {
    const AxrStringIterator returnValue = *this;
    operator++();

    return returnValue;
}

AxrStringIterator& AxrStringIterator::operator--() {
    --m_Character;

    if ((*m_Character & 0b10000000) == 0) {
        // previous character is a 1 byte code point (ascii)
        return *this;
    }

    // If we get here, the previous character must be a multibyte code point. So we keep stepping backwards
    // until we're no longer on a continuation byte
    while (*m_Character >> 6 == 0b00000010) {
        --m_Character;
    }

    return *this;
}

AxrStringIterator AxrStringIterator::operator--(int) {
    const AxrStringIterator returnValue = *this;
    operator--();

    return returnValue;
}

bool AxrStringIterator::operator==(const AxrStringIterator& other) const {
    return m_Character == other.m_Character;
}

bool AxrStringIterator::operator!=(const AxrStringIterator& other) const {
    return !(*this == other);
}

AxrArray<char8_t, 4> AxrStringIterator::operator*() const {
    AxrArray<char8_t, 4> array;
    const char8_t* currentByte = m_Character;
    // Add each byte making up the current character
    do {
        array.pushBack(*currentByte);
        ++currentByte;
        // Continue to loop if the current byte is a continuation byte
    } while (*currentByte >> 6 == 0b00000010);
    // We don't set a null terminator since it's an array of a fixed length. And this will only ever represent a single
    // UTF-8 character

    return array;
}
