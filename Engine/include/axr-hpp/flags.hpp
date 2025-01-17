#pragma once

// This `flags` design have been taken from the vk::Flags in vulkan.hpp.

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <type_traits>

namespace axr {
    template <typename FlagBitsType>
    struct FlagTraits {
        static constexpr bool isBitmask = false;
    };

    template <typename BitType>
    class Flags {
    public:
        using MaskType = typename std::underlying_type<BitType>::type;

        // constructors
        constexpr Flags() noexcept : m_Mask(0) {
        }

        constexpr Flags(BitType bit) noexcept : m_Mask(static_cast<MaskType>(bit)) {
        }

        constexpr Flags(Flags<BitType> const& rhs) noexcept = default;

        constexpr explicit Flags(MaskType flags) noexcept : m_Mask(flags) {
        }

        // relational operators
        auto operator<=>(Flags<BitType> const&) const = default;

        // logical operator
        constexpr bool operator!() const noexcept {
            return !m_Mask;
        }

        // bitwise operators
        constexpr Flags<BitType> operator&(Flags<BitType> const& rhs) const noexcept {
            return Flags<BitType>(m_Mask & rhs.m_Mask);
        }

        constexpr Flags<BitType> operator|(Flags<BitType> const& rhs) const noexcept {
            return Flags<BitType>(m_Mask | rhs.m_Mask);
        }

        constexpr Flags<BitType> operator^(Flags<BitType> const& rhs) const noexcept {
            return Flags<BitType>(m_Mask ^ rhs.m_Mask);
        }

        constexpr Flags<BitType> operator~() const noexcept {
            return Flags<BitType>(m_Mask ^ FlagTraits<BitType>::allFlags.m_mask);
        }

        // assignment operators
        constexpr Flags<BitType>& operator=(Flags<BitType> const& rhs) noexcept = default;

        constexpr Flags<BitType>& operator|=(Flags<BitType> const& rhs) noexcept {
            m_Mask |= rhs.m_Mask;
            return *this;
        }

        constexpr Flags<BitType>& operator&=(Flags<BitType> const& rhs) noexcept {
            m_Mask &= rhs.m_Mask;
            return *this;
        }

        constexpr Flags<BitType>& operator^=(Flags<BitType> const& rhs) noexcept {
            m_Mask ^= rhs.m_Mask;
            return *this;
        }

        // cast operators
        explicit constexpr operator bool() const noexcept {
            return !!m_Mask;
        }

        explicit constexpr operator MaskType() const noexcept {
            return m_Mask;
        }

    private:
        MaskType m_Mask;
    };

    // bitwise operators
    template <typename BitType>
    constexpr Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags) noexcept {
        return flags.operator&(bit);
    }

    template <typename BitType>
    constexpr Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags) noexcept {
        return flags.operator|(bit);
    }

    template <typename BitType>
    constexpr Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags) noexcept {
        return flags.operator^(bit);
    }

    // bitwise operators on BitType
    template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
    constexpr Flags<BitType> operator&(BitType lhs, BitType rhs) noexcept {
        return Flags<BitType>(lhs) & rhs;
    }

    template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
    constexpr Flags<BitType> operator|(BitType lhs, BitType rhs) noexcept {
        return Flags<BitType>(lhs) | rhs;
    }

    template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
    constexpr Flags<BitType> operator^(BitType lhs, BitType rhs) noexcept {
        return Flags<BitType>(lhs) ^ rhs;
    }

    template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
    constexpr Flags<BitType> operator~(BitType bit) noexcept {
        return ~(Flags<BitType>(bit));
    }
}
