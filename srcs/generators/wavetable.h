#pragma once
#include <type_support/array.h>
#include <type_support/constants.h>
#include <span>
#include <cmath>

constexpr auto lerp(const auto &arr, const auto index)
{
    using Float = decltype(index);
    auto i = static_cast<size_t>(index);
    Float t = index - i;
    return arr[i] + (t * (arr[i + 1] - arr[i]));
}


constexpr auto quadratic_interpoation(const auto &arr, const auto index)
{
    using Float = decltype(index);
    auto i = static_cast<size_t>(index) + 1;
    Float t = index - i;
    auto t_squared = t * t;
    auto a = 0.5 * (arr[i + 1] + arr[i - 1]) - arr[i];
    auto b = 0.5 * (arr[i + 1] - arr[i - 1]);
    auto c = arr[i];
    return a * t_squared + b * t + c;
}

template <class FloatType>
struct Wavetable {
    void setFrequency(FloatType freq) {
        phaseIncrement = size * freq * kura::seconds_per_sample;
    }
    auto process_sample([[maybe_unused]] FloatType input) {
        //const auto output = lerp(wavetable, phase);
        const auto output = quadratic_interpoation(wavetable, phase);
        phase += phaseIncrement;
        while (phase > size) phase -= wavetable.size();
        return output;
    }
    FloatType phase {0.0};
    FloatType phaseIncrement {0.0};
    FloatType size = 4096;
    std::span<const FloatType> wavetable;
    /*
    static constexpr auto wavetable = kura::make_array<FloatType>(
        -1.f, 1.f, -1.f
    );
    static constexpr auto size = wavetable.size() - 1;
    */
};
