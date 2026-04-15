// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "stringView.h"

#include <cassert>
#include <string>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrStringView::AxrStringView() = default;

AxrStringView::AxrStringView(const char8_t* string, const size_t size) :
    m_Data(string),
    m_Size(size) {
    assert(size <= std::char_traits<char8_t>::length(string));
}

bool AxrStringView::operator==(const char8_t* srcString) const {
    const size_t srcStringSize = std::char_traits<char8_t>::length(srcString);

    if (srcStringSize != m_Size) {
        return false;
    }

    return std::char_traits<char8_t>::compare(m_Data, srcString, m_Size) == 0;
}

bool AxrStringView::operator==(const AxrStringView& srcString) const {
    return operator==(srcString.data());
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

size_t AxrStringView::size() const {
    return m_Size;
}

bool AxrStringView::empty() const {
    return m_Size == 0;
}

AxrStringView::Iterator AxrStringView::begin() const {
    return Iterator(&m_Data[0]);
}

AxrStringView::Iterator AxrStringView::end() const {
    return Iterator(&m_Data[m_Size]);
}

const char8_t* AxrStringView::data() const {
    return m_Data;
}

AxrStringView AxrStringView::substring(const size_t characterIndex, const size_t count) const {
    Iterator startIterator = end();

    size_t currentCharacterIndex = 0;
    for (Iterator beginIt = begin(), endIt = end(); beginIt != endIt; ++beginIt) {
        if (currentCharacterIndex == characterIndex) {
            startIterator = beginIt;
            break;
        }

        ++currentCharacterIndex;
    }

    return substring(startIterator, count);
}

AxrStringView AxrStringView::substring(const Iterator& startIterator, const size_t count) const {
    Iterator endIterator = end();

    size_t currentCharacterIndex = 0;
    for (Iterator beginIt = startIterator, endIt = end(); beginIt != endIt; ++beginIt) {
        if (currentCharacterIndex == count) {
            endIterator = beginIt;
            break;
        }

        ++currentCharacterIndex;
    }

    return substring(startIterator, endIterator);
}

AxrStringView AxrStringView::substring(const Iterator& startIterator, const Iterator& endIterator) const {
    const size_t substringSize = reinterpret_cast<uintptr_t>(endIterator.getDataPtr()) -
                                 reinterpret_cast<uintptr_t>(startIterator.getDataPtr());

    return AxrStringView(startIterator.getDataPtr(), substringSize);
}
