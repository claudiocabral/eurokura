#include <wavetables/wavetable_generator.h>
#include <cmath>

namespace kura {
template <size_t table_size>
struct table
{
static constexpr auto sine = make_wavetable<table_size>([](auto i, auto size) {
    auto phase = M_PI * (static_cast<float>(i) / static_cast<float>(size));
    return std::sin(phase);
    });
static constexpr auto cosine = make_wavetable<table_size>([](auto i, auto size) {
    auto phase = M_PI * (static_cast<float>(i) / static_cast<float>(size));
    return std::cos(phase);
    });
static constexpr auto square = make_wavetable<table_size>([](auto i, auto size) {
    auto half = size / 2;
    return static_cast<float>(i / half);
    });
static constexpr auto saw = make_wavetable<table_size>([](auto i, auto size) {
    return static_cast<float>(i) / static_cast<float>(size);
    });
static constexpr auto triangle = make_wavetable<table_size>([](auto i, auto size) {
    const auto half = size / 2;
    const auto quarter = size / 4;
    i += quarter;
    while (i > size) i -= size;
    const auto signal = i < half
        ? (static_cast<float>(i) * 2.f) / static_cast<float>(size)
        : 2 - (static_cast<float>(i) * 2.f) / static_cast<float>(size);
    const auto mul = 2.0;
    const auto add = -1.0;
    return signal * mul + add;
    });

 //check what's up (capture of variable with non-automatic storage duration)
static constexpr auto filter = [](auto a) { return a; };
static constexpr auto filtered_saw = make_wavetable<table_size>([](auto i, auto size) {
    return filter(saw[i]);
    });
};
}
