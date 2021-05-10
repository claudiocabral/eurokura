#pragma once
#include <wavetables/wavetables.h>

struct Operator
{
    kura::MultiStageEnvelope envelope;
    kura::Wavetable wavetable;
    float amp = 0.f;
    float feedback = 0.f;
    float ratio = 1.0f;
    float process_sample(float phase)
    {
        return wavetable.process_sample(phase)
            * envelope.process(kura::sample_rate)
            * amp;
    }
};

struct DX7
{
    struct Voice
    {
        std::array<Operator, 6> ops;
        void set_frequency(float freq)
        {
            for(auto & op : ops)
            {
                op.wavetable.set_frequency(freq * op.ratio);
            }
        }
    };
    std::array<Voice, 8> voices;
    unsigned int next_voice = 0;
    static float algo5(Voice *instance)
    {
        auto & op = instance->ops;
        auto mod = std::array {
            op[0].process_sample(0.0),
            op[1].process_sample(0.0),
            op[2].process_sample(0.0),
        };
        return op[3].process_sample(mod[0])
             + op[4].process_sample(mod[1])
             + op[5].process_sample(mod[2]);
    }
    void set_ratio(int op_index, float ratio)
    {
        for (auto & voice: voices)
        {
            voice.ops[op_index].ratio = ratio;
        }
    }
    void set_amplitude(int op_index, float ratio)
    {
        for (auto & voice: voices)
        {
            voice.ops[op_index].ratio = ratio;
        }
    }
};
