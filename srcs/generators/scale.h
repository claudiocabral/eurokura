#pragma once

#include <array>

template <size_t divisions>
constexpr auto make_equal_temperament(float base_freq)
{
    constexpr auto step = 1.0f / static_cast<float>(divisions);
    constexpr auto factor = gcem::pow(2, step);
    std::array<float, divisions + 1> TET;
    for(auto & note : TET)
    {
        note = base_freq;
        base_freq *= factor;
    }
    return TET;
}

constexpr auto select(float base_freq)
{
    auto _12TET = make_equal_temperament<12>(base_freq);
    return std::array {
        _12TET[0],
        _12TET[4],
        _12TET[7],
        _12TET[11],
        _12TET[0] * 2
    };
}

template <typename T, typename... Args>
constexpr auto select_scale_degrees(T && base, Args... args)
{
    return std::array<typename T::value_type, sizeof...(Args)> {
        base[args]...,
    };
}
