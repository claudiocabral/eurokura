#pragma once
#include <math/interpolation.h>
#include <span>

namespace kura
{
    struct MultiStageEnvelope
    {
        float phase{0.f};
        const float * value_table;
        const float * time_table;
        size_t time_table_size;
        auto process(float sample_rate)
        {
            auto ms = sample_rate / 1000.0;
            //auto [i, t] = split_float(phase);
            auto i = static_cast<size_t>(phase);
            auto t = phase - i;
            float not_done = i < time_table_size;
            phase += not_done / ((time_table[i] * ms));
            return lerp(value_table[i + 0], value_table[i + 1], t) * not_done;
        }
        auto done() const
        {
            return phase >= time_table_size;
        }
        auto duration() const
        {
            float output = 0.f;
            for (size_t i = 0; i < time_table_size; ++i) output += time_table[i];
            return output;
        }
        auto reset()
        {
            phase = 0.f;
        }
        static auto make(const auto & levels, const auto & times)
        {
            return kura::MultiStageEnvelope {
                0.f,
                levels.data(),
                times.data(),
                times.size(),
            };

        }
    };
}
