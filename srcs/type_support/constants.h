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

namespace tak {
using float_type = TAK_FLOAT_TYPE;
static constexpr float_type sample_rate = TAK_SAMPLE_RATE;
static constexpr float_type seconds_per_sample = 1.0 / TAK_SAMPLE_RATE;
}
