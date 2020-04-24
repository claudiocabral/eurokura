#pragma once
#include <type_support/array.h>
#include <type_support/constants.h>
#include <cmath>

template <class FloatType>
struct Wavetable {
    void setFrequency(FloatType freq) {
        phaseIncrement = std::sqrt(size * freq * kura::seconds_per_sample);
        std::cout << phaseIncrement << "\n";
    }
    auto process_sample(FloatType input) {
        int current = phase;
        int next = current + 1;
        FloatType remainder = phase - current;
        FloatType output = wavetable[current] * remainder
            + wavetable[next] * (1.0 - remainder);
        phase = std::fmod(phase + phaseIncrement, wavetable.size() - 1);
        return output;
    }
    private:
    FloatType phase {0.0};
    FloatType phaseIncrement {0.0};
    static constexpr auto wavetable = kura::make_array<FloatType>(
        -1.f, 1.f, -1.f
    );
    static constexpr auto size = wavetable.size() - 1;
};
