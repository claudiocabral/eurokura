#pragma once

namespace kura
{
constexpr auto split_float(const auto index)
{
    struct
    {
        using Float = decltype(index);
        size_t integer;
        Float decimal;
    } ret {
        static_cast<size_t>(index),
        index - static_cast<size_t>(index),
    };
    return ret;
}
constexpr auto lerp(const auto &a, const auto & b, const auto normalized_index)
{
    return a + (normalized_index * (b - a));
}

constexpr auto lerp(const auto &arr, const auto index)
{
    auto && [i, t] = split_float(index);
    return lerp(arr[i], arr[i + 1], t);
}
constexpr auto quadratic_interpolation(const auto &a, const auto &b, const auto &c, const auto &t)
{
    auto t_squared = t * t;
    auto x0 = 0.5 * (c + a) - b;
    auto x1 = 0.5 * (c - a);
    auto x2 = b;
    return x0 * t_squared + x1 * t + x2;
}


constexpr auto quadratic_interpolation(const auto &arr, const auto index)
{
    auto && [i, t] = split_float(index + 1);
    return quadratic_interpolation(arr[i - 1], arr[i], arr[i + 1], t);
}
}
