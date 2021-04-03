#include <string>
#include <string_view>

#include <AudioFile.h>

#include <type_support/constants.h>
#include <generators/wavetable.h>
#include <wavetables/wavetables.h>


using File = AudioFile<double>;
constexpr std::string_view filename{"test.wav"};

int main() {
    File file;
    file.setBitDepth(16);
    file.setSampleRate(kura::sample_rate);
    File::AudioBuffer buffer;
    buffer.resize(1);
    auto & channel = buffer.front();
    channel.resize(kura::sample_rate * 10);

    Wavetable<double> w;
    w.wavetable = std::span {
        kura::table<4096>::triangle
    };
    w.setFrequency(440);

    for (auto & sample : channel) {
        constexpr double amplitude = 0.5;
        sample = w.process_sample(0.0) * amplitude;
    }
    file.setAudioBuffer(buffer);
    file.printSummary();
    file.save(std::string(filename));
}
