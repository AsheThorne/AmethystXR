// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "stringView.h"

#include <string>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrStringView::AxrStringView() = default;

AxrStringView::AxrStringView(const char8_t* string, const size_t size) :
    m_Data(string),
    m_Size(size) {
}

bool AxrStringView::operator==(const char8_t* srcString) const {
    const size_t srcStringSize = std::char_traits<char8_t>::length(srcString);

    if (srcStringSize != m_Size) {
        return false;
    }

    for (size_t i = 0; i < m_Size; ++i) {
        if (m_Data[i] != srcString[i]) {
            return false;
        }
    }

    return true;
}

bool AxrStringView::operator==(const AxrStringView& srcString) const {
    if (srcString.m_Size != m_Size) {
        return false;
    }

    for (size_t i = 0; i < m_Size; ++i) {
        if (m_Data[i] != srcString.m_Data[i]) {
            return false;
        }
    }

    return true;
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

const char8_t* AxrStringView::data() const {
    return m_Data;
}
