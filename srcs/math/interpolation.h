#pragma once

namespace kura
{
static constexpr auto split_float(const auto index)
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
static constexpr auto lerp(const auto &a, const auto & b, const auto normalized_index)
{
    return a + (normalized_index * (b - a));
}

static constexpr auto lerp(const auto &arr, const auto index)
{
    auto && [i, t] = split_float(index);
    return lerp(arr[i], arr[i + 1], t);
}

static constexpr auto quadratic_interpolation(const auto &a, const auto &b, const auto &c, const auto &t)
{
    const auto t_squared = t * t;
    const auto x0 = 0.5 * (c + a) - b;
    const auto x1 = 0.5 * (c - a);
    const auto x2 = b;
    return x0 * t_squared + x1 * t + x2;
}


static constexpr auto quadratic_interpolation(const auto &arr, const auto index)
{
    auto i = static_cast<size_t>(index);
    auto t = index - static_cast<size_t>(index);
    return quadratic_interpolation(arr[i - 1], arr[i], arr[i + 1], t);
}
}
