#include <string>
#include <string_view>
#include <random>

#include <AudioFile.h>

#include <type_support/constants.h>
#include <wavetables/wavetables.h>
#include <generators/envelope.h>
#include <generators/wavetable.h>


using File = AudioFile<double>;
constexpr std::string_view filename{"test.wav"};

constexpr auto scale(double base_freq)
{
    auto factor = std::pow(2, 1.0 / 12.0);
    std::array<double, 12> TET;
    for(auto & note : TET)
    {
        note = base_freq;
        base_freq *= factor;
    }
    return std::array {
        TET[0],
        //TET[2],
        TET[4],
        //TET[5],
        TET[7],
        //TET[9],
        TET[11],
        TET[0] * 2
    };
}

constexpr auto major = scale(220);

template <class ValueContainer, class TimeContainer>
struct EnvelopeContainer
{
    ValueContainer envelope_value;
    TimeContainer envelope_time;
    kura::MultiStageEnvelope envelope { 0.0, envelope_value, envelope_time };
};

struct SynthVoice
{
    std::array<double, 4> envelope_value{ 0.0, 1.0, 0.0, 0.0 };
    std::array<double, 2> envelope_time{ 10.0, 100.0 };
    kura::MultiStageEnvelope envelope { 0.0, envelope_value, envelope_time };
    kura::Wavetable<double> wavetable;
    double time;
};

int main() {
    std::random_device rd;
    auto seed = rd();
    std::cout << "seed is " << seed << std::endl;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> env(100, 5000);
    std::uniform_int_distribution<> distrib(0, major.size() - 1);
    File file;
    file.setBitDepth(16);
    file.setSampleRate(kura::sample_rate);
    File::AudioBuffer buffer;
    buffer.resize(1);
    auto & channel = buffer.front();
    double file_duration = 60.0;
    double xfade_begin = 10.0;
    double xfade_end = xfade_begin;
    channel.resize(kura::sample_rate * file_duration);

    kura::MultiStageEnvelope file_envelope;
    auto file_levels = std::array { 0.0, 1.0, 1.0, 0.0, 0.0 };
    auto file_times = std::array { xfade_begin * 1000.0, (file_duration - xfade_begin - xfade_end) * 1000.0, xfade_end * 1000.0 };
    file_envelope.value_table = file_levels;
    file_envelope.time_table = file_times;


    std::array<SynthVoice, 4> voices;


    double now = 0;
    for (auto & voice : voices)
    {
        voice.wavetable.wavetable = std::span {
            kura::table<4096>::sine
        };
        voice.wavetable.setFrequency(440);
        voice.time = -100000.0;
    }
    for (auto & sample : channel)
    {
        sample = 0.0;
        for (auto &voice : voices)
        {
            constexpr double amplitude = 0.5 * 0.25;
            auto & w = voice.wavetable;
            auto & envelope = voice.envelope;
            auto & time = voice.time;
            double duration =  envelope.duration() / 1000.0;
            bool time_ellapsed = now - time > duration;
            if (time_ellapsed)
            {
                time = now;
                envelope.reset();
                envelope.time_table[0] = env(gen);
                envelope.time_table[1] = env(gen);
                w.setFrequency(major[distrib(gen)]);
            }
            double output = w.process_sample(0.0) * amplitude;
            output *= envelope.process(kura::sample_rate);
            sample += output;
        }
        sample *= file_envelope.process(kura::sample_rate);
        now += kura::seconds_per_sample;
    }
    file.setAudioBuffer(buffer);
    file.printSummary();
    file.save(std::string(filename));
}
