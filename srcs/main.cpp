#include <string>
#include <string_view>
#include <random>

#include <AudioFile.h>

#include <type_support/constants.h>
#include <wavetables/wavetables.h>
#include <generators/envelope.h>
#include <generators/wavetable.h>
#include <generators/view_generator.h>
#include <generators/dx7.h>
#include <generators/scale.h>



using File = AudioFile<float>;
constexpr std::string_view filename{"test.wav"};

constexpr auto major_chord = select_scale_degrees(
        make_equal_temperament<12>(220),
        0,
        4,
        7,
        11,
        12
        );

template <class ValueContainer = char[1], class TimeContainer = char[1]>
struct EnvelopeContainer
{
    using Self = EnvelopeContainer<ValueContainer, TimeContainer>;
    ValueContainer values;
    TimeContainer times;
    kura::MultiStageEnvelope envelope;
    auto & operator()() { return envelope; };

    template <class T, class U>
    static constexpr auto make(T && values, U && times)
    {
        return EnvelopeContainer<T, U> {
            .values{values},
            .times{times},
            .envelope{kura::MultiStageEnvelope::make(values, times)},
        };
    }
};

struct SynthVoice
{
    std::array<float, 4> envelope_value{ 0.0, 1.0, 0.0, 0.0 };
    std::array<float, 2> envelope_time{ 10.0, 100.0 };
    kura::MultiStageEnvelope envelope = kura::MultiStageEnvelope::make(
        envelope_value,
        envelope_time
    );
    kura::Wavetable wavetable;
    float time;
    float value;
    float delay = 0.0;
};

int main() {
    std::random_device rd;
    auto seed = rd();
    std::cout << "seed is " << seed << std::endl;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> env(100, 5000);
    std::uniform_int_distribution<> distrib(0, major_chord.size() - 1);
    File file;
    file.setBitDepth(16);
    file.setSampleRate(kura::sample_rate);
    File::AudioBuffer buffer;
    buffer.resize(1);
    auto & channel = buffer.front();
    float file_duration = 60.0f;
    float xfade_begin = 1.0f;
    float xfade_end = xfade_begin;
    channel.resize(kura::sample_rate * file_duration);

    auto file_levels = std::array { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f };
    auto file_times = std::array {
        xfade_begin * 1000.0f,
        (file_duration - xfade_begin - xfade_end) * 1000.0f,
        xfade_end * 1000.0f
    };
    auto file_envelope = kura::MultiStageEnvelope::make(file_levels, file_times);


    std::array<SynthVoice, 4> voices;


    float now = 0;
    for (auto & voice : voices)
    {
        voice.wavetable = kura::Wavetable::make(kura::table<4096>::sine);
        voice.wavetable.set_frequency(440);
        voice.time = -100000.0;
    }
    for (auto & sample : channel)
    {
        sample = 0.0;
        for (auto &voice : voices)
        {
            constexpr float amplitude = 0.5 * 0.25;
            auto & w = voice.wavetable;
            auto & envelope = voice.envelope;
            auto & time = voice.time;

            if (envelope.done())
            {
                time = now;
                envelope.reset();
                voice.envelope_time[0] = env(gen);
                voice.envelope_time[1] = env(gen);
                w.set_frequency(major_chord[distrib(gen)]);
            }
            //attempt at phase feedback
            const auto env = envelope.process(kura::sample_rate);
            auto delay = voice.delay;// * voice.delay;
            auto phase = (delay) * 1024.0 * 0.6;// * env;
            
            float output = w.process_sample(phase);
            voice.delay = output;
            output *= amplitude * env;
            sample += output;
        }
        sample *= file_envelope.process(kura::sample_rate);
        now += kura::sample_duration;
    }
    //file.setAudioBuffer(buffer);
    //file.printSummary();
    //file.save(std::string(filename));
}
