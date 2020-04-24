#include <string>
#include <string_view>

#include <AudioFile.h>

#include <type_support/constants.h>
#include <generators/wavetable.h>


using kura::float_type;
using Generator = Wavetable<float_type>;
using File = AudioFile<float_type>;
constexpr std::string_view filename{"test.wav"};

int main() {
    File file;
    file.setBitDepth(16);
    file.setSampleRate(kura::sample_rate);
    File::AudioBuffer buffer;
    buffer.resize(1);
    auto & channel = buffer.front();
    channel.resize(kura::sample_rate * 10);

    int i = 0;
    Generator generator;
    generator.setFrequency(440);
    for (auto & sample : channel) {
        constexpr float_type amplitude = 0.5;
        sample = generator.process_sample(0.0);
        ++i;
    }
    file.setAudioBuffer(buffer);
    file.printSummary();
    file.save(std::string(filename));
}
