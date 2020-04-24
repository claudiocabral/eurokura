#pragma once
#include <array>

template <class UserData>
class Audio {
    constexpr static float sample_rate = 48'000.0f;
    constexpr static float block_size = 128;
    using audio_array = std::array<float, block_size>;
    auto audioLoop(audio_array & input, audio_array & * output) {
    }
}
