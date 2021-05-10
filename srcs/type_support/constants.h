#pragma once

#ifndef TAK_FLOAT_TYPE
#define TAK_FLOAT_TYPE float
#endif

#ifndef TAK_SAMPLE_RATE
#define TAK_SAMPLE_RATE 48'000.0
#endif

#ifndef TAK_BLOCK_SIZE
#define TAK_BLOCK_SIZE 128
#endif

#include <numbers>

namespace kura {
using float_type = TAK_FLOAT_TYPE;
static constexpr inline float_type sample_rate = TAK_SAMPLE_RATE;
static constexpr inline float_type sample_duration = 1.0 / sample_rate;
static constexpr inline float_type inverse_2pi = 1.0 / (std::numbers::pi_v<float> * 2);
}
