#pragma once
#include <generators/wavetable.h>

namespace kura
{
    struct Envelope
    {
        double attack{10.0};
        double decay{100.0};
        double phase{0.0};
        double value{0.0};
        static constexpr auto table = std::array
        {
            0.0, 1.0, 0.0, 0.0
        };
        auto process(double sample_rate)
        {
            auto ms = sample_rate / 1000.0;
            double increment_stage = 0.0;
            if (phase < 1.0)
            {
                increment_stage = attack;
            }
            else if(phase < 2.0)
            {
                increment_stage = decay;
            }
            else
            {
                return 0.0;
            }
            const auto output = quadratic_interpolation(table, phase);
            phase += 1.0 / (increment_stage * ms);
            return output;
        }
        auto reset()
        {
            phase = 0.0;
        }
    };
}
