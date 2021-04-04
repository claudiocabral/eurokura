#pragma once
#include <type_support/array.h>
#include <type_support/constants.h>
#include <span>
#include <cmath>


namespace kura
{
template <class FloatType>
struct Wavetable {
    void setFrequency(FloatType freq) {
        phaseIncrement = size * freq * kura::seconds_per_sample;
    }
    auto process_sample([[maybe_unused]] FloatType input) {
        const auto output = quadratic_interpolation(wavetable, phase);
        phase += phaseIncrement;
        while (phase > size) phase -= size;
        return output;
    }
    FloatType phase {0.0};
    FloatType phaseIncrement {0.0};
    FloatType size = 4096;
    std::span<const FloatType> wavetable;
};
}
