#pragma once

#include <assert.h>
#include <stdio.h>

namespace plib
{
    #define CONCAT(a, b) CONCAT_INNER(a, b)
    #define CONCAT_INNER(a, b) a ## b

    #define UNIQUE_NAME(base) CONCAT(CONCAT(CONCAT(______, base), __LINE__), _____)

    constexpr void nothing() noexcept
    { }

    template<typename T>
    constexpr T ceil_div(const T& numerator, const T& denominator)
    {
        assert(denominator != 0);
        return ( (numerator + denominator - 1) / denominator );
    }

    template<typename T>
    constexpr T selfxor(T& a, const T& b)
    {
        for(size_t i = 0; i < sizeof(T); ++i)
            reinterpret_cast<unsigned char*>(&a)[i] ^= reinterpret_cast<unsigned char*>(&b)[i];
        return a;
    }
    template<typename T>
    constexpr T xored(T a, const T& b)
    {
        return selfxor(a, b);
    }
}