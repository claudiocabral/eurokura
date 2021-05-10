#pragma once
#include <type_support/array.h>
#include <type_support/constants.h>
#include <math/interpolation.h>
#include <span>
#include <cmath>


namespace kura
{
struct Wavetable {
    void set_frequency(float freq) {
        phaseIncrement = size * freq * kura::sample_duration;
    }
    auto process_sample([[maybe_unused]] float input) {
        auto tmp_phase = phase + input;
        while (tmp_phase < 0) tmp_phase += size;
        while (tmp_phase > size) tmp_phase -= size;
        const auto output = quadratic_interpolation(wavetable, tmp_phase);
        phase += phaseIncrement;
        phase = phase - static_cast<int>(phase / size) *size;
        return output;
    }
    float phase {0.0};
    float phaseIncrement {0.0};
    float size = 0;
    const float * wavetable;
    static constexpr auto make(const float *wavetable, float size)
    {
        return Wavetable{
            .phase{0.0},
            .phaseIncrement{0.0},
            .size{size},
            .wavetable{wavetable},
        };
    }
    template<typename T>
    static constexpr auto make(const T &t)
    {
        return Wavetable{
            .phase{0.0},
            .phaseIncrement{0.0},
            .size{static_cast<float>(t.size())},
            .wavetable{t.data()},
        };
    }
};
}
