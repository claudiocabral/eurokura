#pragma once
#include <math/interpolation.h>
#include <span>

namespace kura
{
    struct MultiStageEnvelope
    {
        double phase{0.0};
        std::span<double> value_table;
        std::span<double> time_table;
        auto process(double sample_rate)
        {
            auto ms = sample_rate / 1000.0;
            auto [i, t] = split_float(phase);
            if (i >= time_table.size()) return 0.0;
            phase += 1.0 / ((time_table[i] * ms));
            return lerp(value_table[i + 0], value_table[i + 1], t);
        }
        auto duration() const
        {
            return std::accumulate(std::begin(time_table), std::end(time_table), 0.0);
        }
        auto reset()
        {
            phase = 0.0;
        }
    };
}
