#pragma once
#include <gcem.hpp>
#include <wavetables/wavetable_generator.h>
#include <cmath>
#include <numbers>

namespace kura {
    inline constexpr float two_pi = std::numbers::pi * 2.0;
template <size_t table_size>
struct table
{
    static constexpr auto center_around_zero(float val) { return val * 2.0 - 1.0; }
    static constexpr auto sine = make_wavetable<table_size>([](auto i, auto size) {
            const auto phase = two_pi * (static_cast<float>(i) / static_cast<float>(size));
            return gcem::sin(phase);
            });
    static constexpr auto cosine = make_wavetable<table_size>([](auto i, auto size) {
            const auto phase = two_pi * (static_cast<float>(i) / static_cast<float>(size));
            return gcem::cos(phase);
            });
    static constexpr auto square = make_wavetable<table_size>([](auto i, auto size) {
            const auto half = size / 2;
            const auto signal = static_cast<float>(i / half);
            return center_around_zero(signal);
            });
    static constexpr auto saw = make_wavetable<table_size>([](auto i, auto size) {
            const auto signal = static_cast<float>(i) / static_cast<float>(size - 1);
            return center_around_zero(signal);
            });
    static constexpr auto triangle = make_wavetable<table_size>([](auto i, auto size) {
            const auto half = size / 2;
            const auto quarter = size / 4;
            i += quarter;
            while (i > size) i -= size;
            const auto signal = i < half
            ? (static_cast<float>(i) * 2.0) / static_cast<float>(size)
            : 2 - (static_cast<float>(i) * 2.0) / static_cast<float>(size);
            return center_around_zero(signal);
            });
};
}
