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

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(10, 250);
    std::uniform_int_distribution<> env(220, 440);
    File file;
    file.setBitDepth(16);
    file.setSampleRate(kura::sample_rate);
    File::AudioBuffer buffer;
    buffer.resize(1);
    auto & channel = buffer.front();
    channel.resize(kura::sample_rate * 10);

    kura::Envelope envelope;
    Wavetable<double> w;
    w.wavetable = std::span {
        kura::table<4096>::triangle
    };

    w.setFrequency(220);

    double time = 0;
    double now = time;
    int i = 0;
    double output = -0.5;
    double duration = (envelope.attack + envelope.decay) / 1000.0;
    for (auto & sample : channel) {
        ++i;
        constexpr double amplitude = 0.5;
        output = w.process_sample(0.0) * amplitude;
        sample = output;
        sample *= envelope.process(kura::sample_rate);
        now += kura::seconds_per_sample;
        bool time_ellapsed = now - time > duration;
        if (time_ellapsed)
        {
            time = now;
            envelope.reset();
            envelope.attack = env(gen);
            envelope.decay = env(gen);
            w.setFrequency(distrib(gen));
            duration = (envelope.attack + envelope.decay) / 1000.0;
        }
    }
    file.setAudioBuffer(buffer);
    file.printSummary();
    file.save(std::string(filename));
}
